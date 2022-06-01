#include "Server.h"
#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPushButton>
#include <QStyleHints>

Server::Server(PlayerManager *player_manager, QWidget *parent)
    : QDialog(parent), statusLabel(new QLabel), m_player_manager(player_manager) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    //    init_server();

    statusLabel->setText(tr("Server is not running"));
    shutdownButton = new QPushButton(tr("Shutdown"));
    hideButton = new QPushButton(tr("Hide"));
    runButton = new QPushButton(tr("Run server"));
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

    QVBoxLayout *mainLayout = nullptr;
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
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::init_server() {
    m_next_id = 2;
    tcpServer = new QTcpServer(this);
    open_connections();
    if (!tcpServer->listen()) {
        QMessageBox::critical(this,
                              tr("Server error"),
                              tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const auto &i : ipAddressesList) {
        if (i != QHostAddress::LocalHost && i.toIPv4Address()) {
            ipAddress = i.toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    //if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::Any).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run remote player now.")
                             .arg(ipAddress)
                             .arg(tcpServer->serverPort()));
}

void Server::destruct_server() {
    tcpServer->deleteLater();
}

void Server::on_shutdown_clicked() {
    statusLabel->setText(tr("Server is not running"));
    destruct_server();
    runButton->setEnabled(true);
    shutdownButton->setEnabled(false);
}

void Server::on_run_clicked() {
    init_server();
    runButton->setEnabled(false);
    shutdownButton->setEnabled(true);
}

void Server::open_connections() const {
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::new_connection);
}

void Server::new_connection() {
    while (tcpServer->hasPendingConnections()) {
        qDebug() << "New connection!\n";
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        qDebug() << "Socket state: " << socket->state() << "\n";
        int id = m_next_id++;
        auto player = new RemotePlayer(tcpServer, socket, id);
        m_player_manager->add_keyboard(id, player->get_keyboard());
        m_player_manager->add_screen(id, player->get_screen());
        connect(player, &RemotePlayer::disconnected, m_player_manager, &PlayerManager::remove_player);

    }
}
