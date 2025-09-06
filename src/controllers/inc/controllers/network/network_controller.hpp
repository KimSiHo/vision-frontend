#pragma once

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QObject>
#include <QTimer>

class NetworkController : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool online READ isOnline NOTIFY onlineChanged)

public:
  explicit NetworkController(QObject* parent = nullptr);
  bool isOnline() const { return is_online_; }

signals:
  void onlineChanged(bool is_online);

private slots:
  void handleOnlineStateChanged(bool is_online);
  void checkInternetReachable();

private:
  bool is_online_;
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  QNetworkConfigurationManager net_config_manager_;

  #pragma GCC diagnostic pop
  QNetworkAccessManager net_access_manager_;
  QTimer timer_;
};
