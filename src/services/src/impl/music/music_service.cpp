#include "services/music/music_service.hpp"

#include "common/utils/logging.hpp"

void MusicService::setTrackChangedHandler(TrackChangedHandler handler) {
    trackChangedHandler_ = std::move(handler);
}

void MusicService::onTrackChanged(const std::string& title, const std::string& coverUrl) {
    SPDLOG_SERVICE_INFO("Track changed: {} ({})", title, coverUrl);
    if (trackChangedHandler_) {
        trackChangedHandler_(title, coverUrl);
    }
}
