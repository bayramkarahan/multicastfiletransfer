#ifndef MULTICASTFILETRANSFERCLIENT_H
#define MULTICASTFILETRANSFERCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QMap>
#include <QHostAddress>

#include "protocol.h"

class MulticastFileTransferClient : public QObject
{
    Q_OBJECT

public:
    explicit MulticastFileTransferClient(QObject *parent=nullptr);
    void start();

signals:
    void transferFinished(QString fileName);

private slots:
    void readPending();
    void readCommand();

private:
    void sendAck(quint32 seq);
    void sendDone();
    QString makeUniqueFileName(const QString &fileName);

    void resetSession();

    QUdpSocket socket;        // multicast data
    QUdpSocket ackSocket;     // ACK + DONE
    QUdpSocket commandSocket; // server özel mesaj

    QFile file;

    QString originalFileName;
    QString currentFileName;

    QMap<quint32, QByteArray> buffer;

    quint32 currentFileId = 0;
    quint32 totalPackets  = 0;

    const int CHUNK = 1400 - sizeof(PacketHeader);
};

#endif
