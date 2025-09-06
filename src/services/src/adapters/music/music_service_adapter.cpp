#include "adapters/music/music_service_adapter.hpp"

#include "common/utils/logging.hpp"

MusicServiceAdapter::MusicServiceAdapter(MusicService& service) : service_(service) {}

bool MusicServiceAdapter::handle(const std::string& topic, const app_common::Json& payload) {
  if (topic == "TRACK_CHANGED") {
    try {
      std::string title = payload.value("title", "");
      std::string cover_url = payload.value("cover_url", "");
      SPDLOG_SERVICE_INFO("Track changed: {} ({})", title, cover_url);
      service_.onTrackChanged(title, cover_url);
      return true;
    } catch (const std::exception& e) {
      SPDLOG_SERVICE_ERROR("Failed to parse TRACK_CHANGED payload: {}", e.what());
      return false;
    }
  }
  return false;
}
