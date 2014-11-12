#include "screenform.h"
#include "ui_screenform.h"
#include <QDebug>
#include <adbsocket.h>
#include <QPainter>
#include <QProcess>

ScreenForm::ScreenForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenForm)
{
    ui->setupUi(this);
    this->connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(back()));
    this->connect(ui->left,SIGNAL(clicked()),this,SLOT(left()));
    this->connect(ui->right,SIGNAL(clicked()),this,SLOT(right()));
}

ScreenForm::~ScreenForm()
{
    delete ui;
}

void ScreenForm::paintEvent(QPaintEvent *e)
{
    //qDebug() << "paint shot!!!";
    //qDebug() << temp::ScreenshotImage.byteCount();
    //temp::ScreenshotImage.load("/home/liushuo/Desktop/111.jpg");//test

    if(!temp::ScreenshotImage.isNull())
    {
        QPainter qp(this);
        qp.drawImage(0,0,temp::ScreenshotImage.scaled(1080/3,1920/3),0,0);
        qp.end();
    }
}

void ScreenForm::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << event->pos();
        QString str = "adb shell input touchscreen tap " + QString("%1 %2").arg(event->pos().x()*3).arg(event->pos().y()*3);
        qDebug() << str;
        QProcess p;
        p.start(str);
        p.waitForStarted();
        p.waitForFinished();
    }
}

void ScreenForm::back()
{
    QString str = "adb shell input keyevent BACK";
    QProcess p;
    p.start(str);
    p.waitForStarted();
    p.waitForFinished();
}

void ScreenForm::left()
{
    QString str = "adb shell input touchscreen swipe "+QString("%1 %2 %3 %4").arg(800).arg(800).arg(200).arg(800);
    QProcess p;
    p.start(str);
    p.waitForStarted();
    p.waitForFinished();
}

void ScreenForm::right()
{
    QString str = "adb shell input touchscreen swipe "+QString("%1 %2 %3 %4").arg(200).arg(800).arg(800).arg(800);
    QProcess p;
    p.start(str);
    p.waitForStarted();
    p.waitForFinished();
}
