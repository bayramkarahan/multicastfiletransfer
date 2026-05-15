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
    emit clientAllProgressStart();
    completedClients.clear();
    allClients.clear();

    // allowed varsa direkt ekle
    if(!allowedClients.isEmpty())
        allClients = QSet<QString>(allowedClients.begin(), allowedClients.end());

    sendMeta();
    QThread::msleep(100);

    sendTimer.stop();
    sendTimer.disconnect();
    ///burst=5;
    //interval = burst/2;
    ///interval=2;
    //log(QString("Adaptive Burst: %1 Interval: %2").arg(burst).arg(interval));

    connect(&sendTimer, &QTimer::timeout, this, [this]()
            {

                for(int i = 0; i < burst && currentIndex < currentJob.totalPackets; i++)
                    sendPacket(currentIndex++);

                if(currentIndex >= currentJob.totalPackets)
                {
                    sendTimer.stop();
                    sendEnd();
                    log("END sent → waiting DONE...");
                    doneTimer->start(startNextJobTimeout);
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
            qint64 sentTime;
            s >> sentTime;
            QString ip = sender.toString();
            qint64 now = QDateTime::currentMSecsSinceEpoch();
            qint64 rtt = now - sentTime;
            helloClientRttMap[ip] = rtt;
            //qDebug() << "RTT:" << ip << rtt << "ms";
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

void MulticastServer::sendHello()
{
    QByteArray datagram;

    QDataStream s(&datagram, QIODevice::WriteOnly);
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    s << (quint32)HELLO;
    s << (quint64)now;
    socket.writeDatagram(
        datagram,
        QHostAddress(MULTICAST_IP),PORT);

    log("HELLO Gönderildi");


    QTimer::singleShot(500, this, [this]()
    {

        log("HELLO_REPLY Geldi.");
        calculateRttValues();
        startNextJob();
    });
}

void MulticastServer::calculateRttValues()
{
    qint64 total = 0;
    qint64 maxRtt = 0;

    // RTT hesapla
    for(qint64 rtt : helloClientRttMap)
    {
        total += rtt;
        maxRtt = qMax(maxRtt, rtt);
    }

    // Client yoksa güvenli mod
    if(helloClientRttMap.isEmpty())
    {
        burst = 2;
        interval = 5;
        startNextJobTimeout = 2000;

        qDebug() << "No HELLO_REPLY received";
        qDebug() << "BURST:" << burst;
        qDebug() << "INTERVAL:" << interval;

        return;
    }

    // Ortalama RTT
    qint64 avgRtt =
        total / helloClientRttMap.size();

    // Dengeli RTT
    qint64 effectiveRtt =
        (avgRtt + maxRtt) / 2;

    qDebug() << "AVG RTT:" << avgRtt;
    qDebug() << "MAX RTT:" << maxRtt;
    qDebug() << "EFFECTIVE RTT:" << effectiveRtt;

    // RTT bazlı başlangıç ayarı
    if(effectiveRtt <= 2)
    {
        burst = 12;
        interval = 1;
    }
    else if(effectiveRtt <= 4)
    {
        burst = 10;
        interval = 1;
    }
    else if(effectiveRtt <= 6)
    {
        burst = 8;
        interval = 2;
    }
    else if(effectiveRtt <= 10)
    {
        burst = 6;
        interval = 3;
    }
    else if(effectiveRtt <= 15)
    {
        burst = 4;
        interval = 4;
    }
    else
    {
        burst = 2;
        interval = 5;
    }

    // Ağ tipi tespiti
    networkType = detectNetworkType();

    // WiFi düzeltmesi
    if(networkType == "wifi")
    {
        // WiFi multicast daha hassas
        burst -= 2;

        // Çok agresif olmasın
        interval += 1;

        startNextJobTimeout = 1000;
    }
    else if(networkType == "ethernet")
    {
        startNextJobTimeout = 100;
    }
    else
    {
        startNextJobTimeout = 500;
    }

    // Güvenlik sınırları
    burst = qBound(2, burst, 14);
    interval = qBound(1, interval, 5);

    qDebug() << "Network Type:" << networkType;
    qDebug() << "startNextJobTimeout:" << startNextJobTimeout;
    qDebug() << "BURST:" << burst;
    qDebug() << "INTERVAL:" << interval;
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
