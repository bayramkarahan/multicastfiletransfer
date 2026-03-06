#include "multicastfiletransferclient.h"
#include <zlib.h>
#include <QFileInfo>

QHostAddress group("239.255.0.1");
quint16 dataPort = 45454;
quint16 ackPort  = 45455;

QHostAddress serverIp("192.168.23.240");

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

    commandSocket.bind(50000);

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

    buffer.clear();
    currentFileId = 0;
    totalPackets  = 0;
}

QString MulticastFileTransferClient::makeUniqueFileName(const QString &fileName)
{
    QFileInfo info(fileName);

    QString baseName = info.completeBaseName();
    QString suffix   = info.suffix();
    QString dir      = info.absolutePath();

    QString newName;

    if(suffix.isEmpty())
        newName = dir + "/" + baseName;
    else
        newName = dir + "/" + baseName + "." + suffix;

    int counter = 1;

    while(QFile::exists(newName))
    {
        if(suffix.isEmpty())
            newName = QString("%1/%2(%3)")
                          .arg(dir)
                          .arg(baseName)
                          .arg(counter);
        else
            newName = QString("%1/%2(%3).%4")
                          .arg(dir)
                          .arg(baseName)
                          .arg(counter)
                          .arg(suffix);

        counter++;
    }

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

        // ---------------- META ----------------
        if(h.type == META)
        {
            resetSession();

            currentFileId = h.fileId;
            totalPackets  = h.total;

            QString fileName =
                QString::fromUtf8(
                    d.mid(sizeof(PacketHeader), h.size));

            originalFileName = makeUniqueFileName(fileName);

            currentFileName = originalFileName + ".part";

            file.setFileName(currentFileName);

            if(!file.open(QIODevice::WriteOnly))
            {
                qDebug() << "Dosya açılamadı";
                return;
            }

            qDebug() << "Yeni transfer başladı:"
                     << originalFileName;
        }

        // ---------------- DATA ----------------
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

        // ---------------- END ----------------
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

                qDebug() << "Transfer tamamlandı:"
                         << originalFileName;

                emit transferFinished(originalFileName);

                sendDone();
                resetSession();
            }
        }
    }
}

void MulticastFileTransferClient::sendAck(quint32 seq)
{
    PacketHeader h;
    h.type  = ACK;
    h.fileId = currentFileId;
    h.seq   = seq;
    h.total = totalPackets;
    h.size  = 0;
    h.crc   = 0;

    ackSocket.writeDatagram(
        reinterpret_cast<char*>(&h),
        sizeof(h),
        serverIp,
        ackPort);
}

void MulticastFileTransferClient::sendDone()
{
    PacketHeader h;
    h.type  = DONE;
    h.fileId = currentFileId;
    h.seq   = 0;
    h.total = totalPackets;
    h.size  = 0;
    h.crc   = 0;

    ackSocket.writeDatagram(
        reinterpret_cast<char*>(&h),
        sizeof(h),
        serverIp,
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
