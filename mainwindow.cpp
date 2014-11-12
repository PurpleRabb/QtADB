#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    as = new AdbSocket();
    as->start();
    ScreenForm *sf = new ScreenForm(this);
    this->connect(as,SIGNAL(getShot()),sf,SLOT(update()));
    sf->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    as->deleteLater();
}
