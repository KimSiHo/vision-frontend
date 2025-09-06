#pragma once

#include <thread>
#include <atomic>

#include <QObject>
#include <zmq.hpp>
#include <vision_common/sub_socket.hpp>

class VisionController : public QObject {
    Q_OBJECT

public:
    explicit VisionController(zmq::context_t& ctx, QObject* parent = nullptr);
    ~VisionController();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void detectionsReceived(const QByteArray& jsonData);

private:
    void run();

    SubSocket sub_;
    std::atomic<bool> running_ = false;
    std::thread th_;
};
