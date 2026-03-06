#ifndef MULTICASTFILETRANSFERSERVER_H
#define MULTICASTFILETRANSFERSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QTimer>
#include <QSet>
#include <QMap>
#include <QHostAddress>

#include "protocol.h"

class MulticastFileTransferServer : public QObject
{
    Q_OBJECT

public:
    explicit MulticastFileTransferServer(QObject *parent=nullptr);

    void sendFile(const QString &path);
    void sendPrivateMessage(QHostAddress ip, QString msg);

signals:
    void clientFinished(QHostAddress ip, quint32 fileId);

private slots:
    void processAck();
    void resendTimeout();

private:
    void sendMeta(const QString &fullname);
    void sendChunk(quint32 seq);
    quint32 crc32(const QByteArray &data);

    QUdpSocket dataSocket;
    QUdpSocket ackSocket;

    QTimer resendTimer;

    QFile file;
    QByteArray fileData;

    QSet<quint32> acked;
    QMap<quint32, QByteArray> window;

    QSet<QHostAddress> finishedClients;

    quint32 fileId = 0;
    quint32 totalPackets = 0;
    quint32 base = 0;
    quint32 next = 0;

    const quint32 WINDOW = 64;
    const int CHUNK = 1400 - sizeof(PacketHeader);
};

#endif
