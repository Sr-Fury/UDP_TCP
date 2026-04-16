#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include "client.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onSendButtonClicked();
    void displayNewMessage(const QString &message);
    void updateConnectionStatus(bool connected);

private:
    QLabel *statusBulb;
    QLabel *statusTitle;
    QTextEdit *chatHistory;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    
    Client *tcpClient;
};

#endif // MAINWINDOW_H