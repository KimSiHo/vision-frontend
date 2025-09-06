#include "controllers/bluetooth/bluetooth_controller.hpp"

#include "common/utils/logging.hpp"
#include "config/zmq_config.hpp"

BluetoothController::BluetoothController(SubSocket& sub_socket, QObject* parent)
    : QObject(parent), sub_socket_(sub_socket) {}

BluetoothController::~BluetoothController() { stop(); }

void BluetoothController::start() {
  notifier_ = new QSocketNotifier(sub_socket_.getFd(), QSocketNotifier::Read, this);
  connect(notifier_, &QSocketNotifier::activated, this, &BluetoothController::onActivated);

  SPDLOG_SERVICE_INFO("Listening (event-driven)");
}

void BluetoothController::stop() {
  if (notifier_) {
    notifier_->setEnabled(false);
    notifier_->deleteLater();
    notifier_ = nullptr;
  }
  emit finished();
}

void BluetoothController::onActivated() {
  while (true) {
    auto data = sub_socket_.receivePayload();
    if (data) {
      SPDLOG_SERVICE_DEBUG("Received message: {}", *data);
      emit devicesUpdated(QString::fromStdString(*data));
    }
  }
}
