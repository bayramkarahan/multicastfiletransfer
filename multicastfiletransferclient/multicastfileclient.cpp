#include "multicastfileclient.h"

MulticastClient::MulticastClient(QObject *parent)
    : QObject(parent), totalPackets(0), currentTransferId(0), allowed(true)
{
    lastPercent = -1;
}

void MulticastClient::log(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    qDebug() << ts << msg;
}

QString MulticastClient::getLocalIp()
{
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces())
    {
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            !(iface.flags() & QNetworkInterface::IsRunning))
            continue;

        for (const QNetworkAddressEntry &entry : iface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress::LocalHost)
            {
                return entry.ip().toString();
            }
        }
    }
    return "";
}

void MulticastClient::start()
{
    log("CLIENT START");
    socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,
                           8*1024*1024);
    socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 64*1024*1024);

    socket.bind(QHostAddress::AnyIPv4, PORT, QUdpSocket::ShareAddress);
    socket.joinMulticastGroup(QHostAddress(MULTICAST_IP));

    while(true)
    {
        socket.waitForReadyRead();

        while(socket.hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(socket.pendingDatagramSize());

            QHostAddress sender;
            socket.readDatagram(datagram.data(), datagram.size(), &sender);

            serverAddress = sender;
            processDatagram(datagram, sender);
        }
    }
}

void MulticastClient::processDatagram(const QByteArray &datagram, const QHostAddress &sender)
{
    QDataStream metaStream(datagram);
    quint32 type;
    metaStream >> type;

    // HELLO
    else if(header.type == HELLO)
    {
        QUdpSocket s;
        QByteArray msg;
        QDataStream stream(&msg, QIODevice::WriteOnly);

        stream << (quint32)HELLO_REPLY;
        s.writeDatagram(msg, serverAddress, NACK_PORT);
        qDebug() << "HELLO_REPLY gönderildi";
    }

    // 🔥 META

    if(type == META)
    {
        emit transferProgress(0);
        lastPercent = -1;
        receivedCount = 0;
        quint64 tid;
        QString getFileName, getTempPath, getDestPath;

        TransferType transferType;
        bool overwrite;
        quint32 total;
        QStringList list;

        metaStream >> tid >> getFileName >> getTempPath >>getDestPath>> transferType >> overwrite >> total >> list;

        currentTransferId = tid;
        fileName = getFileName;
        tmpTargetPath = getTempPath;
        destTargetPath=getDestPath;
        totalPackets = total;

        this->overwrite = overwrite;
        this->transferType = transferType;

        allowedClients = list;

        QString myIp = getLocalIp();
        allowed = allowedClients.isEmpty()
               || allowedClients.contains(myIp)
               || allowedClients.contains("0.0.0.0");

        if(!allowed)
        {
            log(QString("NOT IN LIST (%1) → IGNORING").arg(myIp));
            return;
        }

        packets.resize(totalPackets);
        received.fill(false, totalPackets);

        log(QString("META: %1 (%2) overwrite=%3")
                .arg(fileName)
                .arg(static_cast<int>(transferType))
                .arg(overwrite));

        return;
    }

    // 🔥 liste dışıysa hiçbir şey yapma
    if(!allowed)
        return;

    if(datagram.size() < (int)sizeof(PacketHeader))
        return;

    PacketHeader header;
    memcpy(&header, datagram.data(), sizeof(header));

    if(header.transferId != currentTransferId)
        return;

    if(header.type == DATA)
    {
        if(header.index >= (quint32)packets.size())
            return;

        QByteArray data = datagram.mid(sizeof(header), header.dataSize);

        if(!received[header.index])
        {
            received[header.index] = true;
            packets[header.index] = data;

            // progress burada olmalı
            receivedCount++;
            int percent = (receivedCount * 100) / totalPackets;
            if(percent % 10 == 0 && percent != lastPercent)
            {
                lastPercent = percent;
                emit transferProgress(percent);
                sendProgress(percent);
            }
        }
    }
    else if(header.type == END)
    {
        QVector<quint32> missing;

        for(int i=0;i<received.size();i++)
            if(!received[i]) missing.push_back(i);

        if(!missing.isEmpty())
        {
            sendNack(missing, sender);
            return;
        }

        saveFile();
        resetState();
    }
}

void MulticastClient::sendNack(const QVector<quint32>& missing, const QHostAddress &sender)
{
    QUdpSocket nackSocket;

    QByteArray msg;
    QDataStream s(&msg,QIODevice::WriteOnly);

    s << (quint32)NACK;
    s << missing;

    QThread::msleep(2);
    nackSocket.writeDatagram(msg,sender,NACK_PORT);
}

void MulticastClient::saveFile()
{
    QString fullPath = tmpTargetPath + "/" + fileName;
//destTargetPath
    QFileInfo fi(fullPath);

    // 🔥 DİZİN OLUŞTUR
    QDir().mkpath(fi.path());

    if(!overwrite)
        fullPath = generateFileName(fi.path(), fi.fileName());

    QFile out(fullPath);

    if(!out.open(QIODevice::WriteOnly))
    {
        log("File write error");
        return;
    }

    for(const auto &p : packets)
        out.write(p);

    out.close();

    sendDone();

    log("FILE SAVED: " + fullPath);

    emit fileReceived(tmpTargetPath,destTargetPath, fileName,
                      QString::number(currentTransferId),
                      transferType, overwrite);
}

QString MulticastClient::generateFileName(const QString& dir, const QString& baseName)
{
    QString fullPath = dir + "/" + baseName;

    if(!QFile::exists(fullPath))
        return fullPath;

    QFileInfo info(baseName);
    QString name = info.completeBaseName();
    QString ext = info.suffix();

    int i = 1;
    while(true)
    {
        QString newName = QString("%1(%2)").arg(name).arg(i);
        if(!ext.isEmpty())
            newName += "." + ext;

        QString newPath = dir + "/" + newName;

        if(!QFile::exists(newPath))
            return newPath;

        i++;
    }
}

void MulticastClient::sendDone()
{
    QUdpSocket s;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)DONE;
    stream << currentTransferId;

    s.writeDatagram(msg, serverAddress, NACK_PORT);
}

void MulticastClient::debInstallStart()
{
      log("debInstallStart");
    QUdpSocket s;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)DEB_START;
    stream << currentTransferId;

    s.writeDatagram(msg, serverAddress, NACK_PORT);
}

void MulticastClient::debInstallDone(QString status)
{
    log("debInstallDone");
    QUdpSocket s;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)DEB_DONE;
    stream << currentTransferId;
    stream << status;

    s.writeDatagram(msg, serverAddress, NACK_PORT);
}

void MulticastClient::scriptInstallStart()
{
    log("scriptInstallStart");
    QUdpSocket s;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)SCRIPT_START;
    stream << currentTransferId;

    s.writeDatagram(msg, serverAddress, NACK_PORT);
}

void MulticastClient::scriptInstallDone(QString status)
{
    log("scriptInstallDone");
    QUdpSocket s;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)SCRIPT_DONE;
    stream << currentTransferId;
    stream << status;

    s.writeDatagram(msg, serverAddress, NACK_PORT);
}

void MulticastClient::resetState()
{
    packets.clear();
    received.clear();
    totalPackets = 0;
    currentTransferId = 0;
    allowed = true;

    log("READY FOR NEXT FILE");
}


void MulticastClient::sendProgress(int percent)
{
    QUdpSocket s;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)PROGRESS;
    stream << currentTransferId;
    stream << percent;

    s.writeDatagram(msg, serverAddress, NACK_PORT);
}


QString MulticastClient::resolveTargetPath(TransferType type, const QString& customPath)
{
    UserPrivilegeHelper helper;
    SessionInfo info = helper.getActiveSessionInfo();

    /* if (info.valid) {
         qDebug() << "Kullanıcı:" << info.username;
         qDebug() << "UID/GID:" << info.uid << "/" << info.gid;
         qDebug() << "Home:" << info.home;
         qDebug() << "Display:" << info.display;
         qDebug() << "Type:" << info.type;
         qDebug() << "Service:" << info.service;
     }*/

    //QString user = getActiveUser();
    //qDebug() << "Active user:" << user;
    //QString home="/home/"+user+"/";
    QString home = info.home;
    QString desktop = getDesktopPathFromHome(home);

    switch(type)
    {
        case TransferType::FileCopyDesktop:
            return desktop;

        case TransferType::FileCopyHome:
            return home;

    case TransferType::FileCopyTmp:
        return "/tmp";

        case TransferType::FileCopyCustom:
            return customPath;

        default:
            return "/tmp";
    }
}

QString  MulticastClient::getDesktopPathFromHome(const QString &home)
{
    QFile file(home + "/.config/user-dirs.dirs");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd())
        {
            QString line = file.readLine();

            if(line.startsWith("XDG_DESKTOP_DIR"))
            {
                int firstQuote = line.indexOf('"');
                int lastQuote = line.lastIndexOf('"');

                if(firstQuote != -1 && lastQuote != -1 && lastQuote > firstQuote)
                {
                    QString path = line.mid(firstQuote + 1, lastQuote - firstQuote - 1);
                    path.replace("$HOME", home);
                    return path;
                }
            }
        }
    }

    // fallback
    return home + "/Desktop";
}
bool MulticastClient::copyFile(const QString& src, const QString& dstDir, bool overwrite)
{
    QFileInfo info(src);
    QString dst = dstDir + "/" + info.fileName();

    if(overwrite && QFile::exists(dst))
        QFile::remove(dst);

    return QFile::copy(src, dst);
}


QString MulticastClient::getActiveUser()
{
    QProcess process;
    process.start("bash", QStringList() << "-c"
        << "loginctl list-sessions --no-legend | awk '$4==\"seat0\"{print $3}'");

    process.waitForFinished();

    QString output = process.readAllStandardOutput().trimmed();
    return output;
}
