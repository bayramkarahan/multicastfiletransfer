#include <QCoreApplication>

#include "multicastfileclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
     MulticastClient client;

     QString user = client.getActiveUser();
     qDebug() << "Active user:" << user;
     QString home="/home/"+user+"/";

   QString desktop = client.getDesktopPathFromHome(home);

   qDebug() << "home"<<home;
    qDebug() << "desktop"<<desktop;
    // CLIENT için ayrı program olarak çalıştırılacak

    // MulticastClient client;
     //client.start();



     QObject::connect(&client, &MulticastClient::fileReceived,
                      [&client](QString tmpPath, QString destPath, QString name, QString id, TransferType type,bool overWrite)
                      {

                          QString customPath=destPath;
                          QString targetRoot = client.resolveTargetPath(type, customPath);
                          qDebug() << "CLIENT FILE DONE:";
                          qDebug() << "tempTargetPath:" << tmpPath;
                          qDebug() << "destTargetPath:" << destPath;
                          qDebug() << "finalldestTargetPath:" << targetRoot;
                          qDebug() << "Name:" << name;
                          qDebug() << "Type:" <<static_cast<int>(type);
                          qDebug() << "TransferId:" << id;
                          qDebug() << "OverWrite:" << overWrite;

                             QString sourceRoot = tmpPath;          // /tmp
                             QString relativePath = name;        // aaa/a4.docx
                             QString fullSourcePath = sourceRoot + "/" + relativePath;
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
                                     //::chown(fi.filePath().toUtf8().constData(), info.uid, info.gid);
                                     ::chmod(fi.filePath().toUtf8().constData(), 0644);
                                     break;
                                 }

                                case TransferType::DebInstall:
                                 {
                                    client.debInstallStart();
                                    /************************/
                                     QString filePath = fullSourcePath;
                                     QProcess p;
                                     p.start("dpkg", {"-i", filePath});
                                     p.waitForFinished();
                                     client.debInstallDone(p.exitCode() == 0 ? "0" : "1");
                                     /************************/
                                     break;
                                 }

                                 case TransferType::ScriptExecute:
                                 {
                                     QString filePath = fullSourcePath;
                                     QFile::setPermissions(filePath,
                                         QFileDevice::ExeUser | QFileDevice::ReadUser);
                                     //::chown(filePath.toUtf8().constData(), info.uid, info.gid);
                                     client.scriptInstallStart();
                                     QProcess p;
                                     p.start("/bin/bash", QStringList() << filePath);
                                     p.waitForFinished();
                                     client.scriptInstallDone(p.exitCode() == 0 ? "0" : "1");
                                     /************************/
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
