#include <adbsocket.h>
#include <QDebug>

QImage temp::ScreenshotImage;

void AdbSocket::run()
{
    adb_socket = new QTcpSocket();
    adb_socket->connectToHost("127.0.0.1",7777,QTcpSocket::ReadWrite);
    QByteArray imageData;

    if(this->adb_socket->waitForConnected(1000))
    {
        this->adb_socket->waitForReadyRead(1000);
        QString str = adb_socket->readAll();
        if(str.contains("server ok"))
        {
            adb_socket->write("get pic");
            adb_socket->flush();
            int oldsize,newsize;
            while(true && !isStop)
            {
                oldsize = imageData.size();
                this->adb_socket->waitForReadyRead(1000);
                imageData.append(adb_socket->readAll());
                newsize = imageData.size();
                if(oldsize == newsize)
                {
                    adb_socket->write("get pic");
                    oldsize = 0;
                    temp::ScreenshotImage.loadFromData(imageData);
                    imageData.clear();
                    emit getShot();
                    //qDebug() << newsize;
                }
            }
        }
    }
    adb_socket->write("over");
    adb_socket->flush();
    adb_socket->disconnectFromHost();
}

/*void AdbSocket::readData()
{
    qDebug() << adb_socket->readAll();
    //qDebug() << "hello world!!!";
    adb_socket->write("hello server!");
}

void AdbSocket::adb_connect()
{
    qDebug() << "connected!";
}*/

void AdbSocket::stop()
{
    qDebug() << "stop!";
    isStop = true;
}

AdbSocket::~AdbSocket()
{
    qDebug() << "~AdbSocket";
}
