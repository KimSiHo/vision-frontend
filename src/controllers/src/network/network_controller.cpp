#include "controllers/network/network_controller.hpp"

#include <QDebug>
#include <QNetworkReply>

#include "config/app_config.hpp"

NetworkController::NetworkController(QObject* parent) : QObject(parent), is_online_(false) {
  is_online_ = net_config_manager_.isOnline();

  connect(&net_config_manager_, &QNetworkConfigurationManager::onlineStateChanged, this,
          &NetworkController::handleOnlineStateChanged);

  connect(&timer_, &QTimer::timeout, this, &NetworkController::checkInternetReachable);
  timer_.start(app_config::kPingTimeout);
}

void NetworkController::handleOnlineStateChanged(bool is_online) {
  if (is_online_ != is_online) {
    is_online_ = is_online;
    qInfo() << "Network state changed:" << (is_online_ ? "Online" : "Offline");
    emit onlineChanged(is_online_);
  }
}

void NetworkController::checkInternetReachable() {
  QNetworkRequest req{ QUrl(app_config::kPingServer) };
  QNetworkReply* reply = net_access_manager_.get(req);

  connect(reply, &QNetworkReply::finished, this, [=]() {
    bool is_online = (reply->error() == QNetworkReply::NoError);
    if (is_online_ != is_online) {
      is_online_ = is_online;
      qInfo() << "Network state changed:" << (is_online_ ? "Online" : "Offline");
      emit onlineChanged(is_online_);
    }
    reply->deleteLater();
  });
}
