#include "multicastfileserver.h"
#include <QDataStream>

MulticastServer::MulticastServer(QObject *parent)
    : QObject(parent)
{
    transferId = QDateTime::currentMSecsSinceEpoch();

    doneTimer = new QTimer(this);

    doneTimer->setSingleShot(true);

    connect(doneTimer, &QTimer::timeout, this, [this]()
            {
                int totalClient;

                if(allClients.contains("0.0.0.0"))
                    totalClient = allClients.size() - 1;
                else
                    totalClient = allClients.size();

                qDebug() << "Beklenen client:" << totalClient;
                qDebug() << "DONE gelen:" << completedClients.size();

                QSet<QString> missing;

                for(const QString &ip : allClients)
                {
                    if(ip == "0.0.0.0")
                        continue;

                    if(!completedClients.contains(ip))
                        missing.insert(ip);
                }

                if(!missing.isEmpty())
                {
                    qDebug() << "Eksik clientlar:" << missing;
                }

                qDebug() << "Yeni işe geçiliyor...";
                startNextJob();
            });

}

void MulticastServer::log(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    qDebug() << ts << msg;
}

void MulticastServer::start()
{
    log("SERVER START");

    delayUs = detectDefaultDelay();
    log(QString("Initial delay: %1 us").arg(delayUs));

    socket.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 8*1024*1024);
    socket.setSocketOption(QAbstractSocket::MulticastTtlOption, 4);
    scanPath(sourcePath);

    if(jobQueue.isEmpty())
    {
        log("No files found");
        return;
    }

    nackSocket.bind(QHostAddress::AnyIPv4, NACK_PORT, QUdpSocket::ShareAddress);

    connect(&nackSocket,
            &QUdpSocket::readyRead,
            this,
            &MulticastServer::processPendingDatagrams);

    sendHello();
    //return;

    //startNextJob();
}

void MulticastServer::scanPath(const QString &path)
{
    QFileInfo fi(path);

    if(fi.isFile())
    {
        QFile f(path);
        if(!f.open(QIODevice::ReadOnly)) return;

        FileJob job;
        job.absolutePath = path;
        job.relativePath = fi.fileName();
        job.data = f.readAll();
        job.totalPackets = (job.data.size()+PACKET_SIZE-1)/PACKET_SIZE;

        jobQueue.enqueue(job);
    }
    else if(fi.isDir())
    {
        QString basePath = QFileInfo(path).absoluteFilePath();   // 🔥 kritik
        QDir baseDir(basePath);
        QString baseName = QFileInfo(basePath).fileName();       // aaa

        QDirIterator it(basePath, QDir::Files, QDirIterator::Subdirectories);

        while(it.hasNext())
        {
            QString filePath = it.next();

            QFile f(filePath);
            if(!f.open(QIODevice::ReadOnly)) continue;

            FileJob job;
            job.absolutePath = filePath;

            QString rel = baseDir.relativeFilePath(filePath);
            job.relativePath = baseName + "/" + rel;   // doğru çözüm

            job.data = f.readAll();
            job.totalPackets = (job.data.size()+PACKET_SIZE-1)/PACKET_SIZE;

            jobQueue.enqueue(job);
        }
    }
    totalJobCount=jobQueue.size();

}

void MulticastServer::startNextJob()
{
    if(jobQueue.isEmpty())
    {
        log("ALL FILES SENT");

        return;
    }

    currentJob = jobQueue.dequeue();
    currentIndex = 0;

    completedClients.clear();
    allClients.clear();

    // allowed varsa direkt ekle
    if(!allowedClients.isEmpty())
        allClients = QSet<QString>(allowedClients.begin(), allowedClients.end());

    sendMeta();
    QThread::msleep(100);

    sendTimer.stop();
    sendTimer.disconnect();
    int bw = measureNetworkCapacity();
    //burst = calculateBurstFromBandwidth(bw);
    burst=5;
    //interval = burst/2;
    interval=2;
    log(QString("Adaptive Burst: %1 Interval: %2").arg(burst).arg(interval));

    connect(&sendTimer, &QTimer::timeout, this, [this]()
            {

                /*int adaptiveBurst = calculateAdaptiveBurst();
                if(adaptiveBurst != burst) {
                    burst = adaptiveBurst;
                    interval=burst/12;
                    interval = qBound(2, interval, 10);
                    log(QString("Adaptive Burst: %1 Interval: %2").arg(burst).arg(interval));
                    sendTimer.start(interval); // interval değiştir

                }*/

                for(int i = 0; i < burst && currentIndex < currentJob.totalPackets; i++)
                    sendPacket(currentIndex++);

                if(currentIndex >= currentJob.totalPackets)
                {
                    sendTimer.stop();
                    sendEnd();
                    log("END sent → waiting DONE...");

                    completedClients.clear();
                    doneTimer->start(500);
                }
            });

    QTimer::singleShot(100, this, [this]()
                       {
                           sendTimer.start(interval);
                       });
    //sendTimer.start(2);
}

void MulticastServer::sendMeta()
{
    QByteArray msg;
    QDataStream s(&msg, QIODevice::WriteOnly);

    s << (quint32)META;
    s << transferId;
    s << currentJob.relativePath;
    s << targetTempPath;
    s << targetDestinationPath;
    s << transferType;
    s << transferOverwrite;
    s << (quint32)currentJob.totalPackets;
    s << allowedClients;

    socket.writeDatagram(msg, QHostAddress(MULTICAST_IP), PORT);
    if(!allowedClients.isEmpty())
    {
        allClients = QSet<QString>(allowedClients.begin(), allowedClients.end());
    }
    log("META: " + currentJob.relativePath);
    log(QString("File size %1").arg((quint32)currentJob.data.size()));
    log(QString("Total packets %1").arg((quint32)currentJob.totalPackets));
}

void MulticastServer::sendPacket(int index)
{
    QByteArray chunk = currentJob.data.mid(index*PACKET_SIZE, PACKET_SIZE);

    PacketHeader h{DATA, transferId,
                   (quint32)index,
                   (quint32)currentJob.totalPackets,
                   (quint32)chunk.size()};

    QByteArray p;
    p.append((char*)&h,sizeof(h));
    p.append(chunk);

    socket.writeDatagram(p,QHostAddress(MULTICAST_IP),PORT);
}

void MulticastServer::sendEnd()
{
    PacketHeader h{END, transferId, 0,
                   (quint32)currentJob.totalPackets, 0};

    QByteArray p;
    p.append((char*)&h,sizeof(h));

    socket.writeDatagram(p,QHostAddress(MULTICAST_IP),PORT);


}

void MulticastServer::processPendingDatagrams()
{
    QSet<quint32> missingAll;

    while(nackSocket.hasPendingDatagrams())
    {
        QByteArray d;
        d.resize(nackSocket.pendingDatagramSize());

        QHostAddress sender;
        nackSocket.readDatagram(d.data(), d.size(), &sender);

        QDataStream s(d);
        quint32 type;
        s >> type;

        if(type == NACK)
        {
            QVector<quint32> missing;
            s >> missing;

            QString ip = sender.toString();
            allClients.insert(ip);

            log(QString("NACK from %1 missing=%2")
                .arg(ip)
                .arg(missing.size()));

            missingAll.unite(QSet<quint32>(missing.begin(), missing.end()));
        }
        else if(type == DONE)
        {
            quint64 tid;
            s >> tid;

            if(tid == transferId)
            {
                QString ip = sender.toString();

                // 🔥 CLIENT'I HER ZAMAN EKLE


                if(!completedClients.contains(ip))
                {
                    completedClients.insert(ip);

                    log("CLIENT DONE: " + ip);

                    log(QString("allClients: %1 completed: %2")
                        .arg(allClients.size())
                        .arg(completedClients.size()));


                    emit transferFinished(
                        ip,
                        "server",
                        currentJob.relativePath,
                        QString::number(transferId)
                    );

                    /*if(completedClients.size() >= totalClient)
                    {
                        doneTimer->stop();

                        qDebug() << "Tüm clientlar tamamladı.";

                        completedClients.clear();

                        startNextJob();
                    }*/
                }

            }
        }

        else if(type == PROGRESS)
        {
            quint64 tid;
            int percent;

            s >> tid >> percent;

            if(tid == transferId)
            {
                QString ip = sender.toString();
                clientProgress[ip] = percent;
                allClients.insert(ip);
                emit clientProgressChanged(ip, percent);
            }
        }

        else if(type == DEB_START)
        {
            quint64 tid;
            s >> tid;

            if(tid == transferId)
            {
                QString ip = sender.toString();
                emit clientDebInstallStart(ip);
            }
        }

        else if(type == DEB_DONE)
        {
            quint64 tid;
            QString status;

            s >> tid >> status;

            if(tid == transferId)
            {
                QString ip = sender.toString();
                emit clientDebInstallDone(ip, status);
            }
        }

        else if(type == SCRIPT_START)
        {
            quint64 tid;
            s >> tid;
            if(tid == transferId)
            {
                QString ip = sender.toString();
                //clientProgress[ip] = percent;
                emit clientScriptInstallStart(ip);
            }
        }

        else if(type == SCRIPT_DONE)
        {
            quint64 tid;
            QString status;

            s >> tid >> status;

            if(tid == transferId)
            {
                QString ip = sender.toString();
                emit clientScriptInstallDone(ip, status);
            }
        }
        else if(type == HELLO_REPLY)
        {
            QString user;
            s >> user;

            QString ip = sender.toString();

            if(!helloClients.contains(ip))
            {
                helloClients.insert(ip);

                qDebug() << "Client bulundu:"
                         << ip;
                        // << user;
            }
        }
    }

    if(!missingAll.isEmpty())
    {
        log(QString("Resend %1 packets").arg(missingAll.size()));

        for(auto idx: missingAll)
        {
            sendPacket(idx);
            QThread::usleep(100);
            //QThread::msleep(2);

        }

        sendEnd();
    }
}

int MulticastServer::detectDefaultDelay()
{
    QString type = detectNetworkType();

    if(type == "wifi")
    {
        log("Network: WIFI");
        return 250;
    }
    else if(type == "ethernet")
    {
        log("Network: ETHERNET");
        return 80;
    }

    log("Network: UNKNOWN");
    return 150;
}

QString MulticastServer::getDefaultInterface()
{
    QFile f("/proc/net/route");
    if(!f.open(QIODevice::ReadOnly))
        return "";

    while(!f.atEnd())
    {
        QByteArray line = f.readLine();
        QList<QByteArray> parts = line.split('\t');

        if(parts.size() > 1 && parts[1] == "00000000")
        {
            return parts[0];
        }
    }

    return "";
}

QString MulticastServer::detectNetworkType()
{
    QString activeIface;

    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces())
    {
        //log(QString("%1 | %2")
        //   .arg(iface.name())
        // .arg(iface.humanReadableName()));

        // sadece çalışanlar
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            !(iface.flags() & QNetworkInterface::IsRunning))
            continue;

        // loopback skip
        if (iface.flags() & QNetworkInterface::IsLoopBack)
            continue;

        // IP var mı?
        for (const QNetworkAddressEntry &entry : iface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                activeIface = iface.name();
                log("ACTIVE IFACE: " + activeIface);

                // 🔥 Linux naming standard
                if (activeIface.startsWith("wl"))
                    return "wifi";

                if (activeIface.startsWith("en") || activeIface.startsWith("eth"))
                    return "ethernet";

                return "unknown";
            }
        }
    }

    return "unknown";
}

int MulticastServer::calculateBurst(int delayUs)
{
    if(delayUs <= 50)   return 8;
    if(delayUs <= 100)  return 6;
    if(delayUs <= 200)  return 5;
    return 3;
}


int MulticastServer::calculateAdaptiveBurst()
{
    int total = allClients.size();
    int done  = completedClients.size();

    if(total == 0)
        return burst;

    double ratio = (double)done / total;

    int newBurst = burst;

    if(ratio < 0.3)
        newBurst -= 8;
    else if(ratio < 0.6)
        newBurst -= 4;
    else if(ratio < 0.9)
        newBurst += 2;
    else
        newBurst += 4;

    newBurst = qBound(16, newBurst, 64);

    return newBurst;
}


int MulticastServer::measureNetworkCapacity()
{
    const int testDurationMs = 500;
    const int packetSize = 1200; // gerçek paketine yakın olsun

    QByteArray dummy(packetSize, 'A');

    QElapsedTimer timer;
    timer.start();

    int sentBytes = 0;

    while(timer.elapsed() < testDurationMs)
    {
        socket.writeDatagram(dummy, QHostAddress(MULTICAST_IP), PORT);
        sentBytes += packetSize;
    }

    // bytes/sec
    double bytesPerSec = (sentBytes * 1000.0) / testDurationMs;

    log(QString("Measured throughput: %1 KB/s").arg(bytesPerSec / 1024.0));

    return static_cast<int>(bytesPerSec);
}

int MulticastServer::calculateBurstFromBandwidth(int bytesPerSec)
{
    // saniyede kaç paket?
    int packetsPerSec = bytesPerSec / 1200;

    // timer 2ms → saniyede 500 tick
    int ticksPerSec = 1000 / 2;

    int burst = packetsPerSec / ticksPerSec;

    if(burst < 2) burst = 2;
    if(burst > 10) burst = 10;

    return burst;
}


void MulticastServer::sendHello()
{
    QByteArray datagram;

    QDataStream s(&datagram, QIODevice::WriteOnly);

    s << (quint32)HELLO;
    //s << QDateTime::currentMSecsSinceEpoch();

    socket.writeDatagram(
        datagram,
        QHostAddress(MULTICAST_IP),PORT);

    qDebug() << "HELLO gönderildi";
}
