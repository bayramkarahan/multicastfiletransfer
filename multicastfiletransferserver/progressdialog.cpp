#include "progressdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QLabel>

ProgressDialog::ProgressDialog(const QStringList &clients, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Multicast Dosya Transfer İzleyici");
    resize(600, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // =========================
    // TOTAL PROGRESS BAR
    // =========================
    totalBar = new QProgressBar;
    totalBar->setRange(0, 100);
    totalBar->setValue(0);
    totalBar->setFixedHeight(14);
    totalBar->setAlignment(Qt::AlignCenter);
    totalBar->setFormat("TOTAL: %p%");
    mainLayout->addWidget(totalBar);

    // =========================
    // SCROLL AREA
    // =========================
    scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    scroll->setMinimumHeight(0);
    scroll->setMaximumHeight(150);
    // Scroll barları kapat
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    container = new QWidget;
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    container->setMinimumHeight(0);
    //container->setMaximumHeight(200);

    listLayout = new QVBoxLayout(container);

    listLayout->setSpacing(1);
    listLayout->setContentsMargins(2,2,2,0);

    // önemli: en altta placeholder
    listLayout->addStretch();

    container->setLayout(listLayout);
    scroll->setWidget(container);

    mainLayout->addWidget(scroll);

    int count = progressBars.size();
    int newHeight = calcHeight(count);
    //scroll->setFixedHeight(newHeight);
    container->setFixedHeight(count*20);
    this->resize(this->width(), newHeight);
}

void ProgressDialog::updateProgress(const QString &ip, int percent)
{
    // =========================
    // CLIENT YOKSA OLUŞTUR
    // =========================
    if(!progressBars.contains(ip))
    {
        QHBoxLayout *row = new QHBoxLayout;
        row->setSpacing(2);
        row->setContentsMargins(0,0,0,0);

        QLabel *label = new QLabel(ip);
        label->setFixedWidth(120);

        QProgressBar *bar = new QProgressBar;
        bar->setRange(0, 100);
        bar->setValue(0);
        bar->setFixedHeight(12);
        bar->setAlignment(Qt::AlignCenter);
        bar->setFormat("%p%");

        bar->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #888;"
            "border-radius: 3px;"
            "background: #2b2b2b;"
            "color: white;"
            "}"
            "QProgressBar::chunk { background-color: #4caf50; }"
        );

        row->addWidget(label);
        row->addWidget(bar);

        // stretch üstünde ekle
        listLayout->insertLayout(listLayout->count() - 1, row);

        progressBars[ip] = bar;
        clientPercents[ip] = 0;
        // WINDOW HEIGHT UPDATE
        int count = progressBars.size();
        int newHeight = calcHeight(count);

       // if(count*20>yukseklik)
       //     scroll->setFixedHeight(count*20);
       // else
            scroll->setFixedHeight(count*19);

        container->setFixedHeight(count*20);
        //this->resize(this->width(), newHeight);
        this->resize(this->width(), count*20);

    }

    // =========================
    // UPDATE
    // =========================
    progressBars[ip]->setValue(percent);
    clientPercents[ip] = percent;
    // =========================
    // TOTAL AVERAGE
    // =========================
}

void ProgressDialog::updateTotalProgressBars(double percent)
{
    int sum = 0;
    for(auto v : clientPercents)
        sum += v;

    totalBar->setValue(clientPercents.isEmpty() ? 0 : sum / clientPercents.size());
     totalBar->setValue(percent);
}

void ProgressDialog::markDone(const QString &ip)
{
    if(progressBars.contains(ip))
    {
        progressBars[ip]->setValue(100);
        //progressBars[ip]->setFormat("DONE");

        progressBars[ip]->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #888;"
            "border-radius: 3px;"
            "background: #2b2b2b;"
            "color: white;"
            "}"
            "QProgressBar::chunk { background-color: #2196f3; }"
        );

        //clientPercents[ip] = 100;
        clientPercents[ip] =0;
        int sum = 0;
        for(auto v : clientPercents)
            sum += v;

      ///  totalBar->setValue(clientPercents.isEmpty() ? 0 : sum / clientPercents.size());
    }
}

void ProgressDialog::markDebInstallStart(const QString &ip)
{
    if(progressBars.contains(ip))
    {
        progressBars[ip]->setValue(100);
        progressBars[ip]->setFormat("DEB INSTALL START BEKLEYİN");

        progressBars[ip]->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #888;"
            "border-radius: 3px;"
            "background: #2b2b2b;"
            "color: white;"
            "}"
            "QProgressBar::chunk { background-color: #2196f3; }"
        );
/*
        clientPercents[ip] = 100;

        int sum = 0;
        for(auto v : clientPercents)
            sum += v;

        totalBar->setValue(clientPercents.isEmpty() ? 0 : sum / clientPercents.size());
    */
    }
}

void ProgressDialog::markDebInstallDone(const QString &ip,const QString &status)
{
    if(progressBars.contains(ip))
    {
        progressBars[ip]->setValue(100);
        progressBars[ip]->setFormat("DEB INSTALL DONE");


        progressBars[ip]->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #888;"
            "border-radius: 3px;"
            "background: #2b2b2b;"
            "color: white;"
            "}"
            "QProgressBar::chunk { background-color: #2196f3; }"
        );

       /* clientPercents[ip] = 100;

        int sum = 0;
        for(auto v : clientPercents)
            sum += v;

        totalBar->setValue(clientPercents.isEmpty() ? 0 : sum / clientPercents.size());
    */}
}

void ProgressDialog::markScriptInstallStart(const QString &ip)
{
    if(progressBars.contains(ip))
    {
        progressBars[ip]->setValue(100);
        progressBars[ip]->setFormat("SCRIPT INSTALL START BEKLEYİN");


        progressBars[ip]->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #888;"
            "border-radius: 3px;"
            "background: #2b2b2b;"
            "color: white;"
            "}"
            "QProgressBar::chunk { background-color: #2196f3; }"
        );
/*
        clientPercents[ip] = 100;

        int sum = 0;
        for(auto v : clientPercents)
            sum += v;

        totalBar->setValue(clientPercents.isEmpty() ? 0 : sum / clientPercents.size());
    */}
}

void ProgressDialog::markScriptInstallDone(const QString &ip,const QString &status)
{
    if(progressBars.contains(ip))
    {
        progressBars[ip]->setValue(100);
        progressBars[ip]->setFormat("SCRIPT INSTALL START DONE");


        progressBars[ip]->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #888;"
            "border-radius: 3px;"
            "background: #2b2b2b;"
            "color: white;"
            "}"
            "QProgressBar::chunk { background-color: #2196f3; }"
        );

        /*clientPercents[ip] = 100;

        int sum = 0;
        for(auto v : clientPercents)
            sum += v;

        totalBar->setValue(clientPercents.isEmpty() ? 0 : sum / clientPercents.size());
    */
    }
}
