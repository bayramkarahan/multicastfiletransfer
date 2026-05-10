#include "multicastfileserver.h"
#include <QDataStream>

MulticastServer::MulticastServer(QObject *parent)
    : QObject(parent)
{
    transferId = QDateTime::currentMSecsSinceEpoch();
}

void MulticastServer::log(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    qDebug() << ts << msg;
}

void MulticastServer::start()
{
    log("SERVER START");

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

    startNextJob();
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
            job.relativePath = baseName + "/" + rel;   // 🔥 tek doğru çözüm

            job.data = f.readAll();
            job.totalPackets = (job.data.size()+PACKET_SIZE-1)/PACKET_SIZE;

            jobQueue.enqueue(job);
        }
    }
}

void MulticastServer::startNextJob()
{
    if(jobQueue.isEmpty())
    {
        log("ALL FILES SENT");
        //if(allClients.size()==completedClients.size()){
        allTransferFinished();//}
        return;
    }

    currentJob = jobQueue.dequeue();
    currentIndex = 0;

    completedClients.clear();
    allClients.clear();

    // 🔥 allowed varsa direkt ekle
    if(!allowedClients.isEmpty())
        allClients = QSet<QString>(allowedClients.begin(), allowedClients.end());

    sendMeta();

    sendTimer.stop();
    disconnect(&sendTimer, nullptr, nullptr, nullptr);

    connect(&sendTimer, &QTimer::timeout, this, [this]()
    {
        int burst = 5;

        for(int i=0;i<burst && currentIndex<currentJob.totalPackets;i++)
            sendPacket(currentIndex++);

        if(currentIndex >= currentJob.totalPackets)
        {
            sendTimer.stop();
            sendEnd();

            log("END sent → waiting DONE...");
        }
    });

    sendTimer.start(1);
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
                allClients.insert(ip);

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

                    // 🔥 HER ZAMAN NEXT (senin kullanım için)
                    QTimer::singleShot(50, this, [this]()
                    {
                        startNextJob();
                    });
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

                emit clientProgressChanged(ip, percent);
            }
        }

        else if(type == DEB_START)
        {
            quint64 tid;
            QString status;

            s >> tid >> status;

            if(tid == transferId)
            {
                QString ip = sender.toString();
                emit clientDebInstallStart(ip, status);
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
            QString status;

            s >> tid >> status;

            if(tid == transferId)
            {
                QString ip = sender.toString();
                //clientProgress[ip] = percent;
                emit clientScriptInstallStart(ip, status);
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
    }

    if(!missingAll.isEmpty())
    {
        log(QString("Resend %1 packets").arg(missingAll.size()));

        for(auto idx: missingAll)
        {
            sendPacket(idx);
            QThread::usleep(100);
        }

        sendEnd();
    }
}
