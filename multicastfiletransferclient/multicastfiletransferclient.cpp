#include "multicastfiletransferclient.h"
#include <QNetworkInterface>
#include <QFileInfo>
#include <zlib.h>

QHostAddress group("239.255.0.1");
quint16 dataPort = 45454;
quint16 ackPort  = 45455;
quint16 clientCmdPort = 50000;

MulticastFileTransferClient::MulticastFileTransferClient(QObject *parent)
    : QObject(parent)
{
    socket.bind(QHostAddress::AnyIPv4,
                dataPort,
                QUdpSocket::ShareAddress |
                QUdpSocket::ReuseAddressHint);

    socket.joinMulticastGroup(group);

    connect(&socket,
            &QUdpSocket::readyRead,
            this,
            &MulticastFileTransferClient::readPending);

    ackSocket.bind(0);

    commandSocket.bind(clientCmdPort);

    connect(&commandSocket,
            &QUdpSocket::readyRead,
            this,
            &MulticastFileTransferClient::readCommand);
}

void MulticastFileTransferClient::start()
{
}

void MulticastFileTransferClient::resetSession()
{
    if(file.isOpen())
    {
        file.flush();
        file.close();
    }

    currentFileId = 0;
    totalPackets  = 0;
}

QString MulticastFileTransferClient::makeUniqueFileName(const QString &fileName)
{
    if(!QFile::exists(fileName))
        return fileName;

    QFileInfo info(fileName);
    QString base = info.completeBaseName();
    QString suffix = info.suffix();
    QString dir = info.absolutePath();

    int counter = 1;
    QString newName;

    do
    {
        if(suffix.isEmpty())
            newName = QString("%1/%2(%3)")
                        .arg(dir).arg(base).arg(counter);
        else
            newName = QString("%1/%2(%3).%4")
                        .arg(dir).arg(base).arg(counter).arg(suffix);

        counter++;

    } while(QFile::exists(newName));

    return newName;
}

void MulticastFileTransferClient::readPending()
{
    while(socket.hasPendingDatagrams())
    {
        QByteArray d;
        d.resize(socket.pendingDatagramSize());
        socket.readDatagram(d.data(), d.size());

        if(d.size() < (int)sizeof(PacketHeader))
            continue;

        PacketHeader h;
        memcpy(&h, d.constData(), sizeof(PacketHeader));

        // ================= META =================
        if(h.type == META)
        {
            resetSession();

            currentServerIp = QHostAddress(h.serverIp);

            bool overwrite = (h.overwrite == 1);

            bool allowed = false;

            if(h.targetCount == 0)
                allowed = true;
            else
            {
                const char *ptr =
                    d.constData() + sizeof(PacketHeader);

                for(quint32 i=0;i<h.targetCount;i++)
                {
                    quint32 addr;
                    memcpy(&addr,
                           ptr + i*sizeof(quint32),
                           sizeof(quint32));

                    const auto localAddrs =
                        QNetworkInterface::allAddresses();

                    for(const auto &local : localAddrs)
                    {
                        if(local.protocol() == QAbstractSocket::IPv4Protocol &&
                           local.toIPv4Address() == addr)
                        {
                            allowed = true;
                            break;
                        }
                    }

                    if(allowed)
                        break;
                }
            }

            if(!allowed)
                return;

            int ipListSize =
                h.targetCount * sizeof(quint32);

            int offset = sizeof(PacketHeader)
                         + ipListSize;

            QString fileName =
                QString::fromUtf8(d.mid(offset, h.size));

            offset += h.size;

            QString savePath =
                QString::fromUtf8(d.mid(offset, h.pathSize));

            offset += h.pathSize;

            QString alternativeName =
                QString::fromUtf8(d.mid(offset, h.altNameSize));

            offset += h.altNameSize;

            QString userName =
                QString::fromUtf8(d.mid(offset, h.userNameSize));

            offset += h.userNameSize;

            QString fileType =
                QString::fromUtf8(d.mid(offset, h.fileTypeSize));
            lastAlternativeName = alternativeName;
            lastUserName = userName;
            lastFileType = fileType;

            if(savePath.isEmpty())
                savePath = QDir::homePath();

            QString baseName =
                QFileInfo(fileName).fileName();

            if(overwrite)
            {
                originalFileName =
                    savePath + "/" + baseName;

                if(QFile::exists(originalFileName))
                    QFile::remove(originalFileName);
            }
            else
            {
                originalFileName =
                    makeUniqueFileName(
                        savePath + "/" + baseName);
            }

            currentFileName =
                originalFileName + ".part";

            file.setFileName(currentFileName);

            if(!file.open(QIODevice::WriteOnly))
                return;

            currentFileId = h.fileId;
            totalPackets  = h.total;

            qDebug() << "Transfer başladı:"
                     << originalFileName;
        }

        // ================= DATA =================
        else if(h.type == DATA)
        {
            if(h.fileId != currentFileId)
                continue;

            if(!file.isOpen())
                continue;

            QByteArray payload =
                d.mid(sizeof(PacketHeader), h.size);

            if(::crc32(0L,
                       reinterpret_cast<const Bytef*>(payload.constData()),
                       payload.size()) == h.crc)
            {
                file.seek(h.seq * CHUNK);
                file.write(payload);

                sendAck(h.seq);
            }
        }

        // ================= END =================
        else if(h.type == END)
        {
            if(h.fileId != currentFileId)
                continue;

            if(file.isOpen())
            {
                file.flush();
                file.close();

                QFile::rename(currentFileName,
                              originalFileName);


                emit transferFinished(originalFileName,
                                      lastAlternativeName,
                                      lastUserName,
                                      lastFileType);

                sendDone();
                resetSession();

                qDebug() << "Transfer tamamlandı:"
                         << originalFileName;
            }
        }
    }
}

void MulticastFileTransferClient::sendAck(quint32 seq)
{
    PacketHeader h{};
    h.type  = ACK;
    h.fileId = currentFileId;
    h.seq   = seq;

    ackSocket.writeDatagram(
        reinterpret_cast<char*>(&h),
        sizeof(h),
        currentServerIp,
        ackPort);
}

void MulticastFileTransferClient::sendDone()
{
    PacketHeader h{};
    h.type  = DONE;
    h.fileId = currentFileId;

    ackSocket.writeDatagram(
        reinterpret_cast<char*>(&h),
        sizeof(h),
        currentServerIp,
        ackPort);
}

void MulticastFileTransferClient::readCommand()
{
    while(commandSocket.hasPendingDatagrams())
    {
        QByteArray d;
        d.resize(commandSocket.pendingDatagramSize());
        commandSocket.readDatagram(d.data(), d.size());
        qDebug() << "Server command:" << d;
    }
}
