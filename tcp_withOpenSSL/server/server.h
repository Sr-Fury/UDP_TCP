#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslConfiguration>

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendMessage(const QString &message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void messageReceived(const QString &message);
    void clientConnected(); 
    void clientDisconnected();
    void connectionChanged(bool connected);

private slots:
    void onReadyRead();
    void onClientDisconnectedSlot();
    void onSslErrors(const QList<QSslError> &errors);

private:
    QSslSocket *clientSocket;
    QSslConfiguration sslConfig;
};

#endif