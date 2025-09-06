#include "controllers/network/network_controller.hpp"

#include <QNetworkReply>
#include <QDebug>

#include "config/app_config.hpp"

NetworkController::NetworkController(QObject *parent)
    : QObject(parent),
      isOnline_(false) {
    isOnline_ = netConfigManager_.isOnline();

    connect(&netConfigManager_, &QNetworkConfigurationManager::onlineStateChanged,
            this, &NetworkController::handleOnlineStateChanged);

    connect(&timer_, &QTimer::timeout, this, &NetworkController::checkInternetReachable);
    timer_.start(AppConfig::PING_TIMEOUT);
}

void NetworkController::handleOnlineStateChanged(bool isOnline) {
    if (isOnline_ != isOnline) {
        isOnline_ = isOnline;
        qInfo() << "Network state changed:" << (isOnline_ ? "Online" : "Offline");
        emit onlineChanged(isOnline_);
    }
}

void NetworkController::checkInternetReachable() {
    QNetworkRequest req(QUrl(AppConfig::PING_SERVER));
    QNetworkReply *reply = netAccessManager_.get(req);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        bool isOnline = (reply->error() == QNetworkReply::NoError);
        if (isOnline_ != isOnline) {
            isOnline_ = isOnline;
            qInfo() << "Network state changed:" << (isOnline_ ? "Online" : "Offline");
            emit onlineChanged(isOnline_);
        }
        reply->deleteLater();
    });
}
