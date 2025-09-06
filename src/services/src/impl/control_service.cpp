#include "services/control_service.hpp"

#include "common/utils/logging.hpp"
#include "common/utils/json.hpp"
#include "adapters/music/music_service_adapter.hpp"
#include "config/zmq_config.hpp"

ControlService::ControlService(SubSocket& subSocket)
    : subSocket_(subSocket) {}

void ControlService::registerMusicService(MusicService& svc) {
    services_.push_back(new MusicServiceAdapter(svc));
}

void ControlService::poll() {
    zmq::pollitem_t items[] = {
        { subSocket_.handle(), 0, ZMQ_POLLIN, 0 }
    };

    while (true) {
        zmq::poll(items, 1, -1);
        if (items[0].revents & ZMQ_POLLIN) {
            auto msg = subSocket_.receive();
            SPDLOG_SERVICE_INFO("hi333");

            if (!msg) {
                continue;
            }
            const auto& [topic, payload] = *msg;
            SPDLOG_SERVICE_INFO("Received event: [{}] {}", topic, payload);

            for (auto* svc : services_) {
                svc->handle(topic, AppCommon::json::parse(payload));
            }
        }
    }
}