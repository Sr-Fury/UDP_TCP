#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendMessage(const QString &message);
    bool isClientConnected() const;

signals:
    void messageReceived(const QString &message);
    void clientConnected(); 
    void clientDisconnected();
    void connectionChanged(bool connected);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnectedSlot();

private:
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
};

#endif // SERVER_H