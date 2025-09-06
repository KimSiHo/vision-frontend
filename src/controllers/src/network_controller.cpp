#include "controllers/network_controller.hpp"

#include <QNetworkReply>
#include <QDebug>

#include "config/app_config.hpp"

NetworkController::NetworkController(QObject *parent)
    : QObject(parent), m_online(false) {

    m_online = m_configManager.isOnline();

    connect(&m_configManager, &QNetworkConfigurationManager::onlineStateChanged,
            this, &NetworkController::handleOnlineStateChanged);

    connect(&m_timer, &QTimer::timeout, this, &NetworkController::checkInternetReachable);
    m_timer.start(10000);
}

void NetworkController::handleOnlineStateChanged(bool online) {
    if (m_online != online) {
        m_online = online;
        emit onlineChanged(m_online);
    }
}

void NetworkController::checkInternetReachable() {
    QNetworkRequest req(QUrl("https://www.google.com"));
    qInfo() << "checkInternetReachable called";
    QNetworkReply *reply = m_netManager.get(req);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        bool success = (reply->error() == QNetworkReply::NoError);
        if (m_online != success) {
            m_online = success;
            emit onlineChanged(m_online);
        }
        reply->deleteLater();
    });
}
