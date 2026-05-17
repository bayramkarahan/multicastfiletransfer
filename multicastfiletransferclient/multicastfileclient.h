#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QDateTime>
#include <QFileInfo>
#include <QNetworkInterface>
#include<QDir>
#include<QStandardPaths>
#include<QProcess>
#include <unistd.h>
#include <sys/stat.h>
#include <QDirIterator>
#include<userprivilegehelper.h>

#define MULTICAST_IP "239.255.7.1"
#define PORT 45454
#define NACK_PORT 45455

#define META 0
#define DATA 1
#define END 2
#define NACK 3
#define DONE 4
#define PROGRESS 5
#define DEB_START         6
#define DEB_DONE          7
#define SCRIPT_START      8
#define SCRIPT_DONE       9
#define HELLO 10
#define HELLO_REPLY 11
#define ALLFILESSENTDONE 12

enum class TransferType {
    FileCopyDesktop,
    FileCopyHome,
    FileCopyTmp,
    FileCopyCustom,
    DebInstall,
    ScriptExecute
};
struct PacketHeader
{
    quint32 type;
    quint64 transferId;
    quint32 index;
    quint32 totalPackets;
    quint32 dataSize;
};

class MulticastClient : public QObject
{
    Q_OBJECT
public:
    QString clientHostName;
    explicit MulticastClient(QObject *parent=nullptr);
    void start();
    QString resolveTargetPath(TransferType type, const QString& customPath = "");
    void copyPath(const QString& basePath, const QString& sourceType, TransferType type, const QString& src, const QString& dstDir, bool owrite);

    bool copyFile(const QString& src, const QString& dstDir, bool overwrite);
    bool copyDirectory(const QString &sourceDir,const QString &targetDir,
                       bool overwrite);
    void setPermissionsRecursive(const QString &path, uid_t uid, gid_t gid);

    QString getDesktopPathFromHome(const QString &home);
    void debInstallStart();
    void debInstallDone(QString status);
    void scriptInstallStart();
    void scriptInstallDone(QString status);
    void helloReply(qint64 timestamp);
    QString getActiveUser();
signals:
    void fileReceived(QString tmpPath,
                      QString destPath,
                      QString fileName,
                      QString transferId,
                      TransferType transferType,
                      bool transferOverwrite);
    void transferProgress(int percent);

private slots:
    void log(const QString &msg);
    void processDatagram(const QByteArray &datagram, const QHostAddress &sender);
    void sendNack(const QVector<quint32>& missing, const QHostAddress &sender);
    void saveFile();
    void resetState();
    void sendDone();
    void processPendingDatagrams();

    QString generateFileName(const QString& dir, const QString& baseName);
    void sendProgress(int percent);
    QString getLocalIp();

private:
    QUdpSocket socket;
    QHostAddress serverAddress;

    QVector<QByteArray> packets;
    QVector<bool> received;

    int totalPackets;

    // META
    quint64 currentTransferId;
    QString fileName;
    QString tmpTargetPath;
    QString destTargetPath;
    QString sourceBaseName;
    QString sourceType;

    bool overwrite;
    TransferType transferType;

    // 🔥 YENİ
    QStringList allowedClients;
    bool allowed;
    int receivedCount;
    int lastPercent;
    bool sendAllFilesState=false;
};
