#include "multicastfiletransferserver.h"
#include <QDateTime>
#include <QFileInfo>
#include <QCoreApplication>
#include <zlib.h>

QHostAddress group("239.255.0.1");
quint16 port = 45454;
quint16 ackPort = 45455;
quint16 clientCmdPort = 50000;

MulticastFileTransferServer::MulticastFileTransferServer(QObject *parent)
    : QObject(parent)
{
    dataSocket.setSocketOption(QAbstractSocket::MulticastTtlOption,16);

    ackSocket.bind(QHostAddress::AnyIPv4,
                   ackPort,
                   QUdpSocket::ShareAddress);

    connect(&ackSocket,&QUdpSocket::readyRead,
            this,&MulticastFileTransferServer::processAck);

    resendTimer.setInterval(200);

    connect(&resendTimer,&QTimer::timeout,
            this,&MulticastFileTransferServer::resendTimeout);
}

void MulticastFileTransferServer::sendFile(const QString &path)
{
    finishedClients.clear();

    file.setFileName(path);

    if(!file.open(QIODevice::ReadOnly))
        return;

    fileData = file.readAll();
    file.close();

    fileId = QDateTime::currentSecsSinceEpoch();

    totalPackets = (fileData.size() / CHUNK) + 1;

    QFileInfo fi(path);

    sendMeta(fi.fileName());

    resendTimer.start();

    while(base < totalPackets)
    {
        while(next < base + WINDOW && next < totalPackets)
            sendChunk(next++);

        QCoreApplication::processEvents();
    }

    PacketHeader h;
    h.type = END;
    h.fileId = fileId;
    h.seq = 0;
    h.total = totalPackets;
    h.size = 0;
    h.crc = 0;

    dataSocket.writeDatagram(
        reinterpret_cast<char*>(&h),
        sizeof(h),
        group,
        port);

    resendTimer.stop();
}

void MulticastFileTransferServer::sendMeta(const QString &fullname)
{
    QByteArray name = fullname.toUtf8();

    PacketHeader h;

    h.type = META;
    h.fileId = fileId;
    h.seq = 0;
    h.total = totalPackets;
    h.size = name.size();
    h.crc = 0;

    QByteArray d;

    d.append(reinterpret_cast<char*>(&h),sizeof(h));
    d.append(name);

    dataSocket.writeDatagram(d,group,port);
}

void MulticastFileTransferServer::sendChunk(quint32 seq)
{
    QByteArray chunk = fileData.mid(seq * CHUNK, CHUNK);

    PacketHeader h;

    h.type = DATA;
    h.fileId = fileId;
    h.seq = seq;
    h.total = totalPackets;
    h.size = chunk.size();
    h.crc = crc32(chunk);

    QByteArray d;

    d.append(reinterpret_cast<char*>(&h),sizeof(h));
    d.append(chunk);

    window[seq] = d;

    dataSocket.writeDatagram(d,group,port);
}

void MulticastFileTransferServer::processAck()
{
    while(ackSocket.hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(ackSocket.pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        ackSocket.readDatagram(data.data(),
                               data.size(),
                               &sender,
                               &senderPort);

        if(data.size() < sizeof(PacketHeader))
            continue;

        PacketHeader h;
        memcpy(&h,data.constData(),sizeof(PacketHeader));

        if(h.type == ACK)
        {
            acked.insert(h.seq);

            while(acked.contains(base))
            {
                acked.remove(base);
                window.remove(base);
                base++;
            }
        }
        else if(h.type == DONE)
        {
            if(!finishedClients.contains(sender))
            {
                finishedClients.insert(sender);

                qDebug()<<"Transfer tamamlandı ->"
                         <<sender.toString();

                emit clientFinished(sender,h.fileId);
            }
        }
    }
}

void MulticastFileTransferServer::resendTimeout()
{
    for(auto it=window.begin(); it!=window.end(); ++it)
    {
        if(!acked.contains(it.key()))
        {
            dataSocket.writeDatagram(it.value(),
                                     group,
                                     port);
        }
    }
}

void MulticastFileTransferServer::sendPrivateMessage(QHostAddress ip,
                                                     QString msg)
{
    QUdpSocket s;

    s.writeDatagram(msg.toUtf8(),
                    ip,
                    clientCmdPort);
}

quint32 MulticastFileTransferServer::crc32(const QByteArray &data)
{
    return ::crc32(0L,
                   reinterpret_cast<const Bytef*>(data.constData()),
                   data.size());
}
