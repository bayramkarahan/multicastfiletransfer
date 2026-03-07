#include "multicastfiletransferserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MulticastFileTransferServer server;
    QObject::connect(&server,
                     &MulticastFileTransferServer::clientFinished,
                     [&](QHostAddress client, quint32 fileId)
                     {
                         qDebug() << "Client tamamladı:" << client.toString()
                                  << "fileId:" << fileId;

                         // burada client'a özel işlem yapabilirsin
                     });

    QList<QHostAddress> selected;
    selected << QHostAddress("192.168.1.103");
    QHostAddress serverIp("192.168.1.102");

    server.sendFile("/home/etapadmin/ab.deb",serverIp, selected,true,"/tmp");
    return a.exec();
}
