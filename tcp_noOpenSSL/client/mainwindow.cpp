#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *topBar = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(10, 5, 10, 5);

    statusBulb = new QLabel(this);
    statusBulb->setFixedSize(20, 20);
    statusBulb->setStyleSheet("QLabel { background-color: #D50000; border: 2px solid #8E0000; border-radius: 10px; }");

    statusTitle = new QLabel("Chat Client", this);
    statusTitle->setStyleSheet("QLabel { font-weight: bold; color: white; }");

    topLayout->addWidget(statusBulb);
    topLayout->addWidget(statusTitle);
    topLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);
    chatHistory->setStyleSheet("QTextEdit { border: 1px solid #CCC; border-radius: 5px; background-color: white; padding: 5px; }");

    QWidget *bottomBar = new QWidget(this);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);
    bottomLayout->setContentsMargins(10, 5, 10, 5);

    messageInput = new QLineEdit(this);
    messageInput->setStyleSheet("QLineEdit { border: 1px solid #CCC; border-radius: 5px; padding: 5px; }");

    sendButton = new QPushButton("Send", this);
    sendButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; border-radius: 5px; padding: 6px 12px; font-weight: bold; } QPushButton:pressed { background-color: #0056b3; }");

    bottomLayout->addWidget(messageInput);
    bottomLayout->addWidget(sendButton);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(topBar);
    mainLayout->addWidget(chatHistory);
    mainLayout->addWidget(bottomBar);

    setCentralWidget(centralWidget);
    setWindowTitle("Chat Client");
    resize(400, 500);

    tcpClient = new Client(this);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(messageInput, &QLineEdit::returnPressed, this, &MainWindow::onSendButtonClicked); 
    connect(tcpClient, &Client::messageReceived, this, &MainWindow::displayNewMessage);
    connect(tcpClient, &Client::connectionChanged, this, &MainWindow::updateConnectionStatus);

    tcpClient->connectToServer("127.0.0.1", 1234); 
}

void MainWindow::onSendButtonClicked() {
    QString text = messageInput->text();
    if (!text.isEmpty()) {
        chatHistory->append("Me: " + text);
        tcpClient->sendMessage(text);
        messageInput->clear(); 
    }
}

void MainWindow::displayNewMessage(const QString &message) {
    chatHistory->append("Server: " + message);
}

void MainWindow::updateConnectionStatus(bool connected) {
    if (connected) {
        statusBulb->setStyleSheet("QLabel { background-color: #00C853; border: 2px solid #009624; border-radius: 10px; }");
    } else {
        statusBulb->setStyleSheet("QLabel { background-color: #D50000; border: 2px solid #8E0000; border-radius: 10px; }");
    }
}