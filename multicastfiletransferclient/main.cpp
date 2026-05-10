#include <QCoreApplication>

#include "multicastfileclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
     MulticastClient client;

/*QString home = QDir::homePath();
QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
 qDebug() << "home"<<home;
  qDebug() << "desktop"<<desktop;

  UserPrivilegeHelper helper;
   SessionInfo info = helper.getActiveSessionInfo();
*/
  /* if (info.valid) {
       qDebug() << "Kullanıcı:" << info.username;
       qDebug() << "UID/GID:" << info.uid << "/" << info.gid;
       qDebug() << "Home:" << info.home;
       qDebug() << "Display:" << info.display;
       qDebug() << "Type:" << info.type;
       qDebug() << "Service:" << info.service;
   }*/
   /*QString homee = info.home;
   QString desktopp = client.getDesktopPathFromHome(homee);
   qDebug() << "homee"<<homee;
    qDebug() << "desktopp"<<desktopp;*/
    // CLIENT için ayrı program olarak çalıştırılacak

    // MulticastClient client;
     //client.start();



     QObject::connect(&client, &MulticastClient::fileReceived,
                      [&client](QString tmpPath, QString destPath, QString name, QString id, TransferType type,bool overWrite)
                      {
                            UserPrivilegeHelper helper;
                            SessionInfo info = helper.getActiveSessionInfo();
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
                                     ::chown(fi.filePath().toUtf8().constData(), info.uid, info.gid);
                                     ::chmod(fi.filePath().toUtf8().constData(), 0644);
                                     break;
                                 }

                                case TransferType::DebInstall:
                                 {
                                    client.debInstallStart();
                                    /************************/
                                     QString filePath = fullSourcePath;
                                     QProcess *p = new QProcess();

                                     QObject::connect(
                                         p,
                                         QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                                         p,
                                         [&, p](int code, QProcess::ExitStatus status)
                                         {
                                             Q_UNUSED(status)

                                             if(code == 0){
                                                 client.debInstallDone("0");
                                             }else{
                                                 qDebug() << "DEB FAILED:" << p->readAllStandardError();
                                             client.debInstallDone("1");
                                             }
                                             p->deleteLater();
                                         }
                                     );

                                     p->start("dpkg", {"-i", filePath});
                                     /************************/
                                     break;
                                 }

                                 case TransferType::ScriptExecute:
                                 {
                                     QString filePath = fullSourcePath;
                                     QFile::setPermissions(filePath,
                                         QFileDevice::ExeUser | QFileDevice::ReadUser);
                                     ::chown(filePath.toUtf8().constData(), info.uid, info.gid);
                                     client.scriptInstallStart();

                                     QProcess *p = new QProcess();
                                     /************************/
                                     QObject::connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                                                      [&](int code, QProcess::ExitStatus status)
                                     {
                                         Q_UNUSED(status)

                                         if(code == 0){
                                             client.scriptInstallDone("0");
                                         }else{
                                             qDebug() << "SCRIPT FAILED:" << p->readAllStandardError();
                                         client.scriptInstallDone("1");
                                         }
                                         p->deleteLater();
                                     });

                                     p->start("bash", {filePath});
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
