#include "services/control/control_service.hpp"

#include "adapters/music/music_service_adapter.hpp"
#include "common/utils/json.hpp"
#include "common/utils/logging.hpp"
#include "config/zmq_config.hpp"

ControlService::ControlService(SubSocket& sub_socket) : sub_socket_(sub_socket) {}

void ControlService::start() {
  if (running_) return;
  running_ = true;
  thread_ = std::thread(&ControlService::poll, this);
}

void ControlService::stop() {
  if (!running_) return;
  running_ = false;

  if (thread_.joinable()) {
    thread_.join();
  }
}

void ControlService::registerMusicService(MusicService& service) {
  services_.push_back(new MusicServiceAdapter(service));
}

void ControlService::poll() {
  zmq::pollitem_t items[] = {{sub_socket_.handle(), 0, ZMQ_POLLIN, 0}};

  while (running_) {
    zmq::poll(items, 1, -1);
    if (items[0].revents & ZMQ_POLLIN) {
      auto msg = sub_socket_.receive();
      if (!msg) {
        continue;
      }

      const auto& [topic, payload] = *msg;
      SPDLOG_SERVICE_INFO("Received event: [{}] {}", topic, payload);

      for (auto* service : services_) {
        service->handle(topic, app_common::Json::parse(payload));
      }
    }
  }
}
