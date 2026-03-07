#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{
    server = new QTcpServer(this);

    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));

    //start the server
    if(!server->listen(QHostAddress::Any,6164))
        qDebug()<<"server->listen failed: "<< server->errorString();
    else
        qDebug()<<"Server started: "<<server->serverPort();

}

void Server::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    socket->setObjectName("Client Socket");

    connect(socket,SIGNAL(readyRead()),this,SLOT(handleReadyRead()));
}

void Server::handleReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    qDebug()<<"readReady signal received.";
    socket->objectName();
    m_data = socket->readAll();
    emit dataReceived();
}

QByteArray Server::getData()
{
    return m_data;
}

quint16 Server::getPort()
{
    return server->serverPort();
}


