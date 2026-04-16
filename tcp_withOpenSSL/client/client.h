#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QSslError>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(const QString &ip, quint16 port);
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &message); 
    void connectionChanged(bool connected);

private slots:
    void onReadyRead();
    void onEncrypted();
    void onDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    QSslSocket *socket;
};

#endif