#include "controllers/vision/vision_controller.hpp"

#include "common/utils/logging.hpp"
#include "config/zmq_config.hpp"

VisionController::VisionController(SubSocket& subSocket, QObject* parent)
    : QObject(parent),
      subSocket_(subSocket) { }

VisionController::~VisionController() { stop(); }

void VisionController::start() {
    if (running_) return;
    running_ = true;
    th_ = std::thread([this]{ run(); });
}

void VisionController::stop() {
    running_ = false;
    if (th_.joinable()) th_.join();
}

void VisionController::run() {
    SPDLOG_SERVICE_INFO("VisionController listening (SUB)");

    while (running_) {
        auto payload = subSocket_.receivePayload();
        if (!payload) {
            continue;
        }

        SPDLOG_SERVICE_DEBUG("detections: payload={}", *payload);

        QByteArray json = QByteArray::fromStdString(*payload);
        emit detectionsReceived(json);
    }

    SPDLOG_SERVICE_INFO("VisionController stopped");
}

