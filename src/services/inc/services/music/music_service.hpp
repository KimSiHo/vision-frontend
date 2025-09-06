#pragma once

#include <functional>
#include <string>

class MusicService {
public:
  using TrackChangedHandler = std::function<void(const std::string&, const std::string&)>;
  void setTrackChangedHandler(TrackChangedHandler handler);

  void onTrackChanged(const std::string& title, const std::string& cover_url);

private:
  TrackChangedHandler track_changed_handler_;
};
