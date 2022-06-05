#include "server.h"
#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPushButton>
#include <QStyleHints>

Server::Server(PlayerManager *player_manager_, QWidget *parent)
    : QDialog(parent),
      statusLabel(new QLabel(this)),
      portLabel(new QLabel(this)),
      playerManager(player_manager_) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    portLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    //    init_server();
    statusLabel->setText(tr("Server is not running"));
    shutdownButton = new QPushButton(tr("Shutdown"), this);
    hideButton = new QPushButton(tr("Hide"), this);
    runButton = new QPushButton(tr("Run server"), this);
    addressList = new QListWidget(this);
    shutdownButton->setEnabled(false);
    shutdownButton->setAutoDefault(false);
    connect(shutdownButton, &QPushButton::clicked, this, &Server::on_shutdown_clicked);
    connect(hideButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(runButton, &QPushButton::clicked, this, &Server::on_run_clicked);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(runButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(shutdownButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(hideButton);
    buttonLayout->addStretch(1);

    if (QGuiApplication::styleHints()->showIsFullScreen() ||
        QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QVBoxLayout(this);
    }

    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(addressList);
    mainLayout->addWidget(portLabel);
    mainLayout->addLayout(buttonLayout);
    addressList->hide();
    portLabel->hide();

    setWindowTitle(QGuiApplication::applicationDisplayName());
    setFixedSize(mainLayout->sizeHint());
}

void Server::init_server() {
    nextId = START_ID;
    tcpServer = new QTcpServer(this);
    make_connections();
    QHostAddress address(Config::get_value().server_ip.c_str());
    auto port = Config::get_value().server_port;
    if (!tcpServer->listen(address, port)) {
        QMessageBox::critical(this,
                              tr("Server error"),
                              tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        close();
        return;
    }
    if (address.isEqual(QHostAddress::Any)) {
        auto ipAddressesList = QNetworkInterface::allAddresses();
        for (const auto &i : ipAddressesList) {
            if (i != QHostAddress::LocalHost && i.toIPv4Address()) {
                addressList->addItem(i.toString());
            }
        }
    } else {
        addressList->addItem(address.toString());
    }
    statusLabel->setText(tr("The server is running on IP: "));
    portLabel->setText(tr("port: %1").arg(tcpServer->serverPort()));
    addressList->show();
    portLabel->show();
    setFixedSize(mainLayout->sizeHint());
}

void Server::destruct_server() const {
    tcpServer->deleteLater();
}

void Server::on_shutdown_clicked() {
    addressList->clear();
    addressList->hide();
    portLabel->hide();
    statusLabel->setText(tr("Server is not running"));
    destruct_server();
    runButton->setEnabled(true);
    shutdownButton->setEnabled(false);
    setFixedSize(mainLayout->sizeHint());
}

void Server::on_run_clicked() {
    init_server();
    runButton->setEnabled(false);
    shutdownButton->setEnabled(true);
}

void Server::make_connections() const {
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::new_connection);
}

void Server::new_connection() {
    while (tcpServer->hasPendingConnections()) {
        qDebug() << "New connection!\n";
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        qDebug() << "Socket state: " << socket->state() << "\n";
        int id = nextId++;
        auto player = new RemotePlayer(tcpServer, socket, id);
        playerManager->add_keyboard(id, player->get_keyboard());
        playerManager->add_screen(id, player->get_screen());
        connect(player, &RemotePlayer::disconnected, playerManager, &PlayerManager::remove_player);
    }
}
