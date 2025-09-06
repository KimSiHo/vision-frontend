#include "services/music/music_service.hpp"

#include "common/utils/logging.hpp"

void MusicService::setTrackChangedHandler(TrackChangedHandler handler) { track_changed_handler_ = std::move(handler); }

void MusicService::onTrackChanged(const std::string& title, const std::string& cover_url) {
  SPDLOG_SERVICE_INFO("Track changed: {} ({})", title, cover_url);
  if (track_changed_handler_) {
    track_changed_handler_(title, cover_url);
  }
}
