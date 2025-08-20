#include "vision_controller.hpp"

#include <spdlog/spdlog.h>
#include <vision_common/constants.hpp>

VisionController::VisionController(zmq::context_t& ctx, QObject* parent)
    : QObject(parent),
    sub_(ctx, VisionCommon::AI_RESULTS_ENDPOINT, VisionCommon::TOPIC_DETECTIONS) {
    spdlog::info("[Front] SUB connected to {}", VisionCommon::AI_RESULTS_ENDPOINT);
}

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
    spdlog::info("[Front] VisionController listening (SUB)");

    while (running_) {
        auto msg = sub_.receive();
        if (!msg) {
            continue;
        }

        const auto& [topic, payload] = *msg;
        spdlog::debug("[Front] detections: topic={} payload={}", topic, payload);

        QByteArray json = QByteArray::fromStdString(payload);
        emit detectionsReceived(json);
    }

    spdlog::info("[Front] VisionController stopped");
}
