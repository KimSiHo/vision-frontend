#pragma once

#include <QObject>
#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>
#include <QTimer>

class NetworkController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool online READ isOnline NOTIFY onlineChanged)

public:
    explicit NetworkController(QObject *parent = nullptr);
    bool isOnline() const { return isOnline_; }

signals:
    void onlineChanged(bool isOnline);

private slots:
    void handleOnlineStateChanged(bool isOnline);
    void checkInternetReachable();

private:
    bool isOnline_;
    QNetworkConfigurationManager netConfigManager_;
    QNetworkAccessManager netAccessManager_;
    QTimer timer_;
};
