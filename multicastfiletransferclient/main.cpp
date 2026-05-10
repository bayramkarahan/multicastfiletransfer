#include <QCoreApplication>

#include "multicastfileclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    // CLIENT için ayrı program olarak çalıştırılacak

    // MulticastClient client;
     //client.start();

     MulticastClient client;

     QObject::connect(&client, &MulticastClient::fileReceived,
                      [&client](QString tmpPath, QString destPath, QString name, QString id, TransferType type,bool overWrite)
                      {
                          qDebug() << "CLIENT FILE DONE:";
                          qDebug() << "tempTargetPath:" << tmpPath;
                          qDebug() << "destTargetPath:" << destPath;
                          qDebug() << "Name:" << name;
                          qDebug() << "Type:" <<static_cast<int>(type);
                          qDebug() << "TransferId:" << id;
                          qDebug() << "OverWrite:" << overWrite;

                          QString customPath=destPath;

                             QString sourceRoot = tmpPath;          // /tmp
                             QString relativePath = name;        // aaa/a4.docx

                             QString fullSourcePath = sourceRoot + "/" + relativePath;

                             QString targetRoot = client.resolveTargetPath(type, customPath);

                             QString finalTargetPath = targetRoot + "/" + relativePath;

                             switch(type)
                             {
                                 case TransferType::FileCopyDesktop:
                                 case TransferType::FileCopyHome:
                                 case TransferType::FileCopyCustom:
                                 {
                                     QFileInfo fi(finalTargetPath);
                                     QDir().mkpath(fi.path());

                                     client.copyFile(fullSourcePath, fi.path(), overWrite);
                                     break;
                                 }

                                case TransferType::DebInstall:
                                 {
                                     QString filePath = fullSourcePath;
                                     //QProcess::execute("dpkg -i " + filePath);
                                     break;
                                 }

                                 case TransferType::ScriptExecute:
                                 {
                                     QString filePath = fullSourcePath;
                                     QFile::setPermissions(filePath,
                                         QFileDevice::ExeUser | QFileDevice::ReadUser);

                                     //QProcess::startDetached("bash " + filePath);
                                     break;
                                 }
                             }

     });
     QObject::connect(&client, &MulticastClient::transferProgress,
                      [](int p)
     {
         qDebug() << "Progress:" << p << "%";
     });

     //QTimer::singleShot(0, [&](){
         client.start();
     //});
    return a.exec();
}
