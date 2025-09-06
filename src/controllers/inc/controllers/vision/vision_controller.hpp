#pragma once

#include <thread>
#include <atomic>

#include <QObject>

#include "common/zmq/sub_socket.hpp"

class VisionController : public QObject {
    Q_OBJECT

public:
    explicit VisionController(SubSocket& subSocket, QObject* parent = nullptr);
    ~VisionController();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void detectionsReceived(const QByteArray& jsonData);

private:
    void run();

    SubSocket& subSocket_;
    std::atomic<bool> running_ = false;
    std::thread th_;
};
