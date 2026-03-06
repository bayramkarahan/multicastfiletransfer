#include<multicastfiletransferclient.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MulticastFileTransferClient client;
    QObject::connect(&client,
                     &MulticastFileTransferClient::transferFinished,
                     [](QString file)
                     {
                         qDebug() << "Client: transfer tamamlandı ->" << file;
                     });
    client.start();
    return a.exec();
}
