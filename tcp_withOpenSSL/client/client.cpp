#include "client.h"
#include "message_crypto.h"

Client::Client(QObject *parent) : QObject(parent) {
    socket = new QSslSocket(this);

    connect(socket, &QSslSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QSslSocket::encrypted, this, &Client::onEncrypted);
    connect(socket, &QSslSocket::disconnected, this, &Client::onDisconnected);
    connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));
}

void Client::connectToServer(const QString &ip, quint16 port) {
    socket->connectToHostEncrypted(ip, port);
}

void Client::sendMessage(const QString &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8() + "\n";

        apply_xor_cipher(data.data(), data.size());

        socket->write(data);
        socket->flush(); 
    }
}

void Client::onReadyRead() {
    while (socket->canReadLine()) {
        QByteArray data = socket->readLine();

        apply_xor_cipher(data.data(), data.size());
        
        emit messageReceived(QString::fromUtf8(data).trimmed());
    }
}

void Client::onEncrypted() {
    emit connectionChanged(true);
}

void Client::onDisconnected() {
    emit connectionChanged(false); 
}

void Client::onSslErrors(const QList<QSslError> &errors) {
    socket->ignoreSslErrors();
}