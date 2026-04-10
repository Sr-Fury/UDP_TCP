#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(const QString &ip, quint16 port);
    void sendMessage(const QString &message);
    bool isConnected() const;

signals:
    void messageReceived(const QString &message); 
    void connectionChanged(bool connected);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H