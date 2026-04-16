#include "server.h"

Server::Server(QObject *parent) : QObject(parent), clientSocket(nullptr) {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::startServer(quint16 port) {
    return tcpServer->listen(QHostAddress::Any, port);
}

void Server::onNewConnection() {
    clientSocket = tcpServer->nextPendingConnection();
    
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnectedSlot);
    
    emit connectionChanged(true);
    emit clientConnected();
}

void Server::onReadyRead() {
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();
        emit messageReceived(QString::fromUtf8(data).trimmed());
    }
}

void Server::sendMessage(const QString &message) {
    if (clientSocket && clientSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8() + "\n";
        clientSocket->write(data);
    }
}

void Server::onClientDisconnectedSlot() {
    if (clientSocket) {
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }
    emit connectionChanged(false);
    emit clientDisconnected();
}

bool Server::isClientConnected() const {
    return clientSocket && clientSocket->state() == QAbstractSocket::ConnectedState;
}