#include "server.h"
#include <QSslKey>
#include <QFile>
#include <QCoreApplication>

Server::Server(QObject *parent) : QTcpServer(parent), clientSocket(nullptr) {
    QString basePath = QCoreApplication::applicationDirPath();

    QFile certFile(basePath + "/server.crt");
    certFile.open(QIODevice::ReadOnly);
    sslConfig.setLocalCertificate(QSslCertificate(&certFile, QSsl::Pem));
    
    QFile keyFile(basePath + "/server.key");
    keyFile.open(QIODevice::ReadOnly);
    sslConfig.setPrivateKey(QSslKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey));
}

bool Server::startServer(quint16 port) {
    return listen(QHostAddress::Any, port);
}

void Server::incomingConnection(qintptr socketDescriptor) {
    clientSocket = new QSslSocket(this);
    
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        clientSocket->setSslConfiguration(sslConfig);
        clientSocket->startServerEncryption();

        connect(clientSocket, &QSslSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QSslSocket::disconnected, this, &Server::onClientDisconnectedSlot);
        connect(clientSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));

        emit connectionChanged(true);
        emit clientConnected();
    } else {
        delete clientSocket;
        clientSocket = nullptr;
    }
}

void Server::onReadyRead() {
    if (clientSocket) {
        while (clientSocket->canReadLine()) {
            QByteArray data = clientSocket->readLine();
            emit messageReceived(QString::fromUtf8(data).trimmed());
        }
    }
}

void Server::sendMessage(const QString &message) {
    if (clientSocket && clientSocket->isEncrypted()) {
        QByteArray data = message.toUtf8() + "\n";
        clientSocket->write(data);
        clientSocket->flush();
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

void Server::onSslErrors(const QList<QSslError> &errors) {
    if (clientSocket) {
        clientSocket->ignoreSslErrors();
    }
}