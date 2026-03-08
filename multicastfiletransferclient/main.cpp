#include<multicastfiletransferclient.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MulticastFileTransferClient client;
    QObject::connect(&client,
                     &MulticastFileTransferClient::transferFinished,
                     [](QString file,
                        QString alt,
                        QString user,
                        QString type)
    {
        qDebug() << "Dosya:" << file;
        qDebug() << "Alternatif:" << alt;
        qDebug() << "Kullanıcı:" << user;
        qDebug() << "Type:" << type;
    });
    client.start();
    return a.exec();
}
