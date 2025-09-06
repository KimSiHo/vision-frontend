#include "controllers/vision/vision_controller.hpp"

#include "common/utils/logging.hpp"
#include "config/zmq_config.hpp"

VisionController::VisionController(SubSocket& sub_socket, QObject* parent) : QObject(parent), sub_socket_(sub_socket) {}

VisionController::~VisionController() { stop(); }

void VisionController::start() {
  if (running_) return;
  running_ = true;
  thread_ = std::thread([this] { run(); });
}

void VisionController::stop() {
  running_ = false;
  if (thread_.joinable()) thread_.join();
}

void VisionController::run() {
  SPDLOG_SERVICE_INFO("VisionController listening (SUB)");

  while (running_) {
    auto payload = sub_socket_.receivePayload();
    if (!payload) {
      continue;
    }

    SPDLOG_SERVICE_DEBUG("Detections: payload={}", *payload);

    QByteArray json = QByteArray::fromStdString(*payload);
    emit detectionsReceived(json);
  }

  SPDLOG_SERVICE_INFO("VisionController stopped");
}

// void VisionController::enqueue(const std::string& payload) {
//   std::lock_guard<std::mutex> lock(queue_mutex_);
//   queue_.push(payload);
//   queue_cv_.notify_one();
// }

// void VisionController::run() {
//   while (running_) {
//     std::unique_lock<std::mutex> lock(queue_mutex_);
//     queue_cv_.wait(lock, [this] { return !queue_.empty() || !running_; });

// if (!running_) break;

// auto payload = std::move(queue_.front());
// queue_.pop();
// lock.unlock();

// QByteArray json = QByteArray::fromStdString(payload);
// emit detectionsReceived(json);
//}
//}
