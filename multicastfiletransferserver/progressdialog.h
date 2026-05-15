#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include<QDebug>
class QScrollArea;

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(const QStringList &clients, QWidget *parent = nullptr);
    int yukseklik=150;
    void updateProgress(const QString &ip, int percent);
    void allProgressStart();
    void updateTotalProgressBars(double percent);

    void markDone(const QString &ip);
    void markDebInstallStart(const QString &ip);
    void markDebInstallDone(const QString &ip,const QString &status);
    void markScriptInstallStart(const QString &ip);
    void markScriptInstallDone(const QString &ip,const QString &status);


    int calcHeight(int clientCount)
    {
        int baseHeight = 20;   // total bar + padding
        int perItem = 22;

        // en az 1 client varmış gibi davran
        int effectiveCount = qMax(1, clientCount);

        int h =(effectiveCount * perItem);

        if(h > yukseklik+50) h = yukseklik+50;

        return h;
    }

private:
    QMap<QString, QProgressBar*> progressBars;
    QMap<QString, int> clientPercents;

    QProgressBar *totalBar = nullptr;
    QVBoxLayout *listLayout = nullptr;
    QScrollArea *scroll= nullptr;
    QWidget *container = nullptr;

};

#endif
