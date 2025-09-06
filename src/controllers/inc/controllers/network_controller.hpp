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

    bool isOnline() const { return m_online; }

    signals:
        void onlineChanged(bool online);

private slots:
    void handleOnlineStateChanged(bool online);
    void checkInternetReachable();

private:
    bool m_online;
    QNetworkConfigurationManager m_configManager;
    QNetworkAccessManager m_netManager;
    QTimer m_timer;
};
