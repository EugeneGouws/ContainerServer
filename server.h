#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    QByteArray getData();
    quint16 getPort();

public slots:
    void newConnection();
    void handleReadyRead();

private:
    QTcpServer *server;
    QByteArray m_data;

signals:
    void dataReceived();
};

#endif // SERVER_H
