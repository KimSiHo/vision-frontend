#include "controllers/bluetooth_controller.hpp"
#include <spdlog/spdlog.h>

BluetoothController::BluetoothController(zmq::context_t& ctx, QObject* parent)
    : QObject(parent),
      sub_(ctx, zmq::socket_type::sub)
{
    try {
        sub_.connect("ipc:///tmp/bt_scan.sock");
        sub_.set(zmq::sockopt::subscribe, ""); // 모든 메시지 구독
        spdlog::info("[BTController] Connected to ipc:///tmp/bt_scan.sock");
    } catch (const zmq::error_t& e) {
        spdlog::error("[BTController] Failed to connect SUB socket: {}", e.what());
    }
}

BluetoothController::~BluetoothController() {
    stop();
}

void BluetoothController::start() {
    if (running_) return;
    running_ = true;
    th_ = std::thread([this]{ run(); });
}

void BluetoothController::stop() {
    running_ = false;
    if (th_.joinable()) {
        th_.join();
    }
}

void BluetoothController::run() {
    spdlog::info("[BTController] Listening (SUB)");

    while (running_) {
        zmq::message_t msg;
        try {
            if (sub_.recv(msg, zmq::recv_flags::none)) {
                std::string data(static_cast<char*>(msg.data()), msg.size());
                spdlog::debug("[BTController] Received message: {}", data);
                emit devicesUpdated(QString::fromStdString(data));
            }
        } catch (const zmq::error_t& e) {
            spdlog::error("[BTController] recv error: {}", e.what());
        }
    }

    spdlog::info("[BTController] Stopped");
}
