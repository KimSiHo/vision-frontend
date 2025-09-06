#pragma once

#include <QObject>
#include <QSocketNotifier>

#include "common/zmq/sub_socket.hpp"

class BluetoothController : public QObject {
  Q_OBJECT

public:
  explicit BluetoothController(SubSocket& sub_socket, QObject* parent = nullptr);
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
  SubSocket& sub_socket_;
  QSocketNotifier* notifier_{nullptr};
};
