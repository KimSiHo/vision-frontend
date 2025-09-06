#include "network_controller.hpp"
#include <QNetworkReply>
#include <QDebug>
#include "app_config.hpp"


NetworkController::NetworkController(QObject *parent)
    : QObject(parent), m_online(false) {

    // 초기 상태
    m_online = m_configManager.isOnline();

    // Qt 네트워크 상태 감지
    connect(&m_configManager, &QNetworkConfigurationManager::onlineStateChanged,
            this, &NetworkController::handleOnlineStateChanged);

    // 주기적으로 인터넷 연결 체크 (5초마다)
    connect(&m_timer, &QTimer::timeout, this, &NetworkController::checkInternetReachable);
    m_timer.start(5000);
}

void NetworkController::handleOnlineStateChanged(bool online) {
    if (m_online != online) {
        m_online = online;
        emit onlineChanged(m_online);
    }
}

void NetworkController::checkInternetReachable() {
    QNetworkRequest req(QUrl("https://www.google.com")); // 가벼운 ping용
    qInfo() << "checkInternetReachable called";   // 이렇게 써야 함
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
