#pragma once

#include <QObject>
#include <thread>
#include <atomic>
#include <zmq.hpp>

class BluetoothController : public QObject {
    Q_OBJECT

public:
    explicit BluetoothController(zmq::context_t& ctx, QObject* parent = nullptr);
    ~BluetoothController();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    signals:
        void devicesUpdated(const QString& data);

private:
    void run();

    zmq::socket_t sub_;
    std::thread th_;
    std::atomic<bool> running_{false};
};
