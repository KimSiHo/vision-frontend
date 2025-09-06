#include "adapters/music/music_service_adapter.hpp"

#include "common/utils/logging.hpp"

MusicServiceAdapter::MusicServiceAdapter(MusicService& svc)
    : svc_(svc) {}

bool MusicServiceAdapter::handle(const std::string& topic, const AppCommon::json& payload) {
    if (topic == "TRACK_CHANGED") {
        try {
            std::string title    = payload.value("title", "");
            std::string coverUrl = payload.value("cover_url", "");
            SPDLOG_SERVICE_INFO("Track changed: {} ({})", title, coverUrl);
            svc_.onTrackChanged(title, coverUrl);
            return true;
        } catch (const std::exception& e) {
            SPDLOG_SERVICE_ERROR("Failed to parse TRACK_CHANGED payload: {}", e.what());
            return false;
        }
    }
    return false;
}

