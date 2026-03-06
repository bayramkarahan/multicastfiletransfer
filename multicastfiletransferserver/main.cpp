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

    server.sendFile("/home/etapadmin/ab.deb");
    return a.exec();
}
