#pragma once

#include <QObject>
#include <QSocketNotifier>

#include "common/zmq/sub_socket.hpp"

class BluetoothController : public QObject {
    Q_OBJECT

public:
    explicit BluetoothController(SubSocket& subSocket, QObject* parent = nullptr);
    ~BluetoothController();

signals:
    void devicesUpdated(const QString& data);
    void finished();

public slots:
    void start();
    void stop();

private slots:
    void onActivated();

private:
    SubSocket& subSocket_;
    QSocketNotifier* notifier_{nullptr};
};
