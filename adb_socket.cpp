#include <adbsocket.h>
#include <QDebug>

QImage temp::ScreenshotImage;
struct Banner {
    uint version;
    uint length;
    uint pid;
    uint realWidth;
    uint realHeight;
    uint virtualWidth;
    uint virtualHeight;
    uint orientation;
    uint quirks;
};

void AdbSocket::run()
{
    adb_socket = new QTcpSocket();
    adb_socket->connectToHost("127.0.0.1",1313,QTcpSocket::ReadWrite);
    QByteArray chunk;
    uint readBannerBytes = 0;
    uint bannerLength = 2;
    uint readFrameBytes = 0;
    uint frameBodyLength = 0;
    struct Banner banner;
    QByteArray frameBody;

    if(this->adb_socket->waitForConnected(1000))
    {
        while(true)
        {
            //this->adb_socket->waitForReadyRead(100);
            adb_socket->waitForReadyRead();
            int bufflen = adb_socket->bytesAvailable();
            chunk.insert(0,adb_socket->readAll());
            int len = bufflen;
            qDebug() << "total len:" << len;
            for(int cursor = 0; cursor < len;) {
                if(readBannerBytes < bannerLength) {
                    switch (readBannerBytes) {
                    case 0:
                        banner.version = chunk[cursor];
                        break;
                    case 1:
                        banner.length = bannerLength = chunk[cursor];
                        break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        banner.pid += ((unsigned char)chunk[cursor] << ((readBannerBytes - 2) * 8)) >> 0;
                        break;
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        banner.realWidth += ((unsigned char)chunk[cursor] << ((readBannerBytes - 6) *  8)) >> 0;
                        break;
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                        banner.realHeight += ((unsigned char)chunk[cursor] << ((readBannerBytes - 10) * 8)) >> 0;
                        break;
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                        banner.virtualWidth += ((unsigned char)chunk[cursor] << ((readBannerBytes - 14) * 8));
                        break;
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                        banner.virtualHeight += ((unsigned char)chunk[cursor] << ((readBannerBytes - 18) * 8));
                        break;
                    case 22:
                        banner.orientation += chunk[cursor] * 90;
                        break;
                    case 23:
                        banner.quirks = chunk[cursor];
                        break;
                    }
                    cursor += 1;
                    readBannerBytes += 1;
                    if(readBannerBytes == bannerLength) {
                        qDebug() << "banner!!!";
                        qDebug() << "version:" << banner.version;
                        qDebug() << "length:" << banner.length;
                        qDebug() << "pid:" << banner.pid;
                        qDebug() << "realWidth:" << banner.realWidth;
                        qDebug() << "readHeight:" << banner.realHeight;
                        qDebug() << "virtualWidth" << banner.virtualWidth;
                        qDebug() << "virtualHeight" << banner.virtualHeight;
                        qDebug() << "orientation" << banner.orientation;
                    }
                }
                else if(readFrameBytes < 4) {
                    frameBodyLength += ((unsigned char)chunk[cursor] << (readFrameBytes * 8));
                    cursor += 1;
                    readFrameBytes += 1;
                    //qDebug() << "headerbyte:" << readFrameBytes << "framelength:" << frameBodyLength;
                }
                else {
                    if( len - cursor >= frameBodyLength) {
                        //qDebug() << "bodyfin:" << frameBodyLength << "cursor:" << cursor;
                        //deal with the framebody
                        frameBody.insert(frameBody.size(),chunk.mid(cursor,cursor+frameBodyLength));
                        if(frameBody[0] != 0xFF || frameBody[1] != 0xD8) {
                            qDebug() << "Not a jpeg!!!";
                        }
                        cursor += frameBodyLength;
                        frameBodyLength = readFrameBytes = 0;
                        temp::ScreenshotImage.loadFromData(frameBody);
                        emit getShot();
                        frameBody.clear();
                    }
                    else {
                        //qDebug() << "body len:" << len-cursor;
                        frameBody.insert(frameBody.size(),chunk.mid(cursor,len));
                        //temp::ScreenshotImage.loadFromData(frameBody);
                        //emit getShot();
                        frameBodyLength -= len-cursor;
                        readFrameBytes += len-cursor;
                        cursor = len;
                    }
                }
            }
            //temp::ScreenshotImage.loadFromData(frameBody);
            //emit getShot();
            QThread::msleep(10);
            chunk.clear();
            adb_socket->flush();
        }
    }
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
