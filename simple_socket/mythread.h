#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QString>
#include <QThread>
#include <QTcpSocket>
#include "packet.h"

class MyThread : public QThread {
    Q_OBJECT

public:
    int id;
    explicit MyThread(const qintptr desc, QObject *parent = nullptr);
    void run();
    void sendPacket(Packet pkt);

signals:
    void error(QTcpSocket::SocketError err);
    void dataAvailable(Packet);
    void exiting(MyThread*);

public slots:
    void onRecv();
    void onDisconnect();

private:
    QTcpSocket *socket;
    qintptr socket_descriptor;
    Packet pkt;
};

#endif // MYTHREAD_H
