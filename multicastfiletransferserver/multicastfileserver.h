#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QVector>
#include <QThread>
#include <QDateTime>
#include <QSet>
#include <QDebug>
#include <QFileInfo>
#include <QTimer>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QDirIterator>
#include <QQueue>

#define MULTICAST_IP "239.255.7.1"
#define PORT 45454
#define NACK_PORT 45455
#define PACKET_SIZE 1312

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

struct FileJob
{
    QString absolutePath;
    QString relativePath;
    QByteArray data;
    int totalPackets;
};

class MulticastServer : public QObject
{
    Q_OBJECT
public:
    explicit MulticastServer(QObject *parent = nullptr);

    QString sourcePath;
    QString targetTempPath;
    QString targetDestinationPath;

    TransferType transferType;
    bool transferOverwrite;
    QStringList allowedClients;

    void start();

signals:
    void clientProgressChanged(QString client, int percent);
    void clientDebInstallStart(QString client, QString status);
    void clientDebInstallDone(QString client, QString status);
    void clientScriptInstallStart(QString client, QString status);
    void clientScriptInstallDone(QString client, QString status);
    void allTransferFinished();
    void transferFinished(QString sender,
                             QString receiver,
                             QString file,
                             QString id);
private slots:
    void processPendingDatagrams();
    int detectDefaultDelay();
    QString getDefaultInterface();
    QString detectNetworkType();
    int calculateBurst(int delayUs);
    int calculateAdaptiveBurst();

private:
    void log(const QString &msg);
    void sendPacket(int index);
    void sendMeta();
    void sendEnd();
    void startNextJob();
    void scanPath(const QString &path);

    QUdpSocket socket;
    QUdpSocket nackSocket;

    QQueue<FileJob> jobQueue;
    FileJob currentJob;

    int currentIndex = 0;
    quint64 transferId;
    int delayUs =200;
    QTimer sendTimer;
    int burst=5;
    QSet<QString> allClients;
    QSet<QString> completedClients;

    QMap<QString,int> clientProgress;
};
