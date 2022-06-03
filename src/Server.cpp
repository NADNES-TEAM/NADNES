#include "Server.h"
#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPushButton>
#include <QStyleHints>

Server::Server(PlayerManager *player_manager, QWidget *parent)
    : QDialog(parent), statusLabel(new QLabel(this)), m_player_manager(player_manager) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    //    init_server();
    portLabel = new QLabel(this);
    statusLabel->setText(tr("Server is not running"));
    shutdownButton = new QPushButton(tr("Shutdown"), this);
    hideButton = new QPushButton(tr("Hide"), this);
    runButton = new QPushButton(tr("Run server"), this);
    address_list = new QListWidget(this);
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
        main_layout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        main_layout = new QVBoxLayout(this);
    }
    
    main_layout->addWidget(statusLabel);
    main_layout->addWidget(address_list);
    main_layout->addWidget(portLabel);
    main_layout->addLayout(buttonLayout);
    address_list->hide();
    portLabel->hide();

    setWindowTitle(QGuiApplication::applicationDisplayName());
    setFixedSize(main_layout->sizeHint());
}

void Server::init_server() {
    m_next_id = START_ID;
    tcpServer = new QTcpServer(this);
    open_connections();
    QHostAddress address(config::get_value("server.ip", QString("0.0.0.0")));
    auto port = config::get_value("server.port", 0);
    if (!tcpServer->listen(address, port)) {
        QMessageBox::critical(this,
                              tr("Server error"),
                              tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        close();
        return;
    }
    if(address == QHostAddress::Any) {
        auto ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (const auto &i : ipAddressesList) {
            if (i != QHostAddress::LocalHost && i.toIPv4Address()) {
                address_list->addItem(i.toString());
            }
        }
    } else {
        address_list->addItem(address.toString());
    }
    statusLabel->setText(tr("The server is running on IP: "));
    portLabel->setText(tr("port: %1").arg(tcpServer->serverPort()));
    address_list->show();
    portLabel->show();
    setFixedSize(main_layout->sizeHint());
}

void Server::destruct_server() const {
    tcpServer->deleteLater();
}

void Server::on_shutdown_clicked() {
    address_list->clear();
    address_list->hide();
    portLabel->hide();
    statusLabel->setText(tr("Server is not running"));
    destruct_server();
    runButton->setEnabled(true);
    shutdownButton->setEnabled(false);
    setFixedSize(main_layout->sizeHint());
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
        connect(player,
                &RemotePlayer::disconnected,
                m_player_manager,
                &PlayerManager::remove_player);
    }
}
