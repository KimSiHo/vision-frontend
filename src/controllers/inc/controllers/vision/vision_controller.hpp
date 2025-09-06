#pragma once

#include <QObject>
#include <atomic>
#include <thread>

#include "common/zmq/sub_socket.hpp"

class VisionController : public QObject {
  Q_OBJECT

public:
  explicit VisionController(SubSocket& sub_socket, QObject* parent = nullptr);
  ~VisionController();

  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

signals:
  void detectionsReceived(const QByteArray& json_data);

private:
  void run();

  SubSocket& sub_socket_;
  std::atomic<bool> running_{false};
  std::thread thread_;
};
