#include "client.h"

Client::Client(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
}

void Client::connectToServer(const QString &ip, quint16 port) {
    socket->connectToHost(ip, port);
}

void Client::sendMessage(const QString &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8() + "\n";
        socket->write(data);
    }
}

void Client::onReadyRead() {
    QByteArray data = socket->readAll();
    emit messageReceived(QString::fromUtf8(data).trimmed());
}

void Client::onConnected() {
    emit connectionChanged(true);
}

void Client::onDisconnected() {
    emit connectionChanged(false); 
}

bool Client::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}