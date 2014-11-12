#ifndef ADB_SOCKET_H_
#define ADB_SOCKET_H_

#include <qobject.h>
#include <qtcpsocket.h>
#include <QThread>
#include <QMutex>
#include <QImage>

class temp {
public:
    static QImage ScreenshotImage;
};


class AdbSocket:public QThread
{
    Q_OBJECT
public:
     //AdbSocket(QObject *parent = 0);
     ~AdbSocket();
     void run();
     void stop();

private:
    QMutex mutex;
    QTcpSocket *adb_socket;
    bool isStop = false;

signals:
    void getShot();
};

#endif
