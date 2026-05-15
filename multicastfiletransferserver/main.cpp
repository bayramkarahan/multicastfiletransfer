#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include<QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QDir>

#include "multicastfileserver.h"
#include "progressdialog.h"

// --------------------------------------------------
// ARG STRUCT
// --------------------------------------------------
struct ParsedArgs
{
    QString mode = "project";
    QString target = "tmp";
    QString filePath;
};
// -----------------------------
// TARGET PATH RESOLVER
// -----------------------------
// --------------------------------------------------
// ARG PARSER
// --------------------------------------------------
ParsedArgs parseArguments(QStringList args)
{
    ParsedArgs result;

    args.removeFirst(); // program adı

    for(int i = 0; i < args.size(); i++)
    {
        const QString &arg = args[i];

        if(arg == "--mode" && i + 1 < args.size())
        {
            result.mode = args[i + 1].toLower();
            i++;
        }
        else if(arg == "--target" && i + 1 < args.size())
        {
            result.target = args[i + 1].toLower();
            i++;
        }
        else
        {
            if(result.filePath.isEmpty())
                result.filePath = arg;
        }
    }

    return result;
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Kullanım:";
    qDebug() << "multicast_sender --mode console --target home /dosya";

    // -----------------------------
    // ARG PARSE
    // -----------------------------
    ParsedArgs args = parseArguments(QCoreApplication::arguments());

    qDebug() << "Mode:" << args.mode;
    qDebug() << "Target:" << args.target;
    qDebug() << "FilePath:" << args.filePath;

    // -----------------------------
    // SERVER
    // -----------------------------
    MulticastServer server;
    server.targetTempPath = "/tmp";
    server.targetDestinationPath = "/tmp";
    if(args.target=="deb")
        server.transferType = TransferType::DebInstall;
    if(args.target=="script")
        server.transferType = TransferType::ScriptExecute;

    if(args.target=="desktop")
        server.transferType = TransferType::FileCopyDesktop;
    if(args.target=="home")
        server.transferType = TransferType::FileCopyHome;
    if(args.target=="tmp")
        server.transferType = TransferType::FileCopyTmp;
    if(args.target=="custom")
    {
        server.transferType = TransferType::FileCopyCustom;
        bool ok = false;
        QString dir = QInputDialog::getText(
            nullptr,
            "Hedef Konum",
            "Dosya nereye kopyalansın?\n"
            "Örnek:\n"
            "/home/user/Desktop\n"
            "/tmp\n"
            "/opt/data",
            QLineEdit::Normal,
            QDir::homePath(),
            &ok
        );

        if(!ok || dir.isEmpty())
        {
            qDebug() << "Kullanıcı iptal etti";
            return -1;
        }

        // (opsiyonel) basit doğrulama
        if(!dir.startsWith("/"))
        {
            qDebug() << "Geçersiz path";
            return -1;
        }
        server.targetDestinationPath = dir;
        qDebug() << "Custom target:" << server.targetDestinationPath;
    }


    server.transferOverwrite = true;
    // -----------------------------
    // MODE LOGIC
    // -----------------------------
    if(args.mode == "rightclick" || args.mode == "console")
    {

        if( args.filePath.isEmpty())
                {
                    qDebug() << "Hata: Dosya yolu yok!";
                    return -1;
                }
        server.sourcePath = args.filePath;
        // herkes kabul
        server.allowedClients.clear();
        server.allowedClients << "0.0.0.0";
    }
    else // project
    {
        server.sourcePath = "./aaa";
        server.allowedClients
                << "0.0.0.0";
    }

    qDebug() << "SourcePath:" << server.sourcePath;
    qDebug() << "TargetPath:" << server.targetTempPath;
    qDebug() << "AllowedClients:" << server.allowedClients;
    qDebug() << "TransferType:" << static_cast<int>(server.transferType);
    // -----------------------------
    // UI
    // -----------------------------
    ProgressDialog *dlg = new ProgressDialog(server.allowedClients);
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    // -----------------------------
    // SIGNALS
    // -----------------------------
    QObject::connect(&server, &MulticastServer::transferFinished,
                     [&](QString sender, QString receiver, QString file, QString id)
    {
        Q_UNUSED(receiver)
        Q_UNUSED(file)
        Q_UNUSED(id)

        qDebug() << "Client tamamladı:" << sender;
        dlg->markDone(sender);
    });

    QObject::connect(&server, &MulticastServer::allTransferFinished,
                     [&]()
    {
        qDebug() << "Tüm transfer tamamlandı";
        QTimer::singleShot(5000, qApp, []()
        {
            ///qApp->quit();
        });
    });

    QObject::connect(&server,&MulticastServer::clientProgressChanged,
                     [&,&server](QString ip, int percent)
    {
        dlg->updateProgress(ip, percent);

        int processed = server.totalJobCount - server.jobQueue.size();
        double totalpercent =(double(processed) / server.totalJobCount) * 100.0;
        dlg->updateTotalProgressBars(totalpercent);

    });

    QObject::connect(&server,&MulticastServer::clientDebInstallStart,
                     [&](QString ip)
    {
        dlg->markDebInstallStart(ip);
    });
    QObject::connect(&server,&MulticastServer::clientDebInstallDone,
                     [&](QString ip, QString status)
    {
        dlg->markDebInstallDone(ip, status);
    });
    QObject::connect(&server,&MulticastServer::clientScriptInstallStart,
                     [&](QString ip)
    {
        dlg->markScriptInstallStart(ip);
    });
    QObject::connect(&server,&MulticastServer::clientScriptInstallDone,
                     [&](QString ip, QString status)
    {
        dlg->markScriptInstallDone(ip, status);
    });
    // -----------------------------
    // START
    // -----------------------------
    QTimer::singleShot(0, [&]()
    {
        server.start();
    });
    /*dlg->updateProgress("192.168.1.190", 20);
    dlg->updateProgress("192.168.1.191", 30);
    dlg->updateProgress("192.168.1.192", 60);
    dlg->updateProgress("192.168.1.193", 70);
    dlg->updateProgress("192.168.1.194", 80);*/

    dlg->show();

    return a.exec();
}
