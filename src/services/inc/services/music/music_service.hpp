#pragma once

#include <string>
#include <functional>

class MusicService {
public:
    // callback 설정
    using TrackChangedHandler = std::function<void(const std::string&, const std::string&)>;
    void setTrackChangedHandler(TrackChangedHandler handler);

    // 서비스 API
    void onTrackChanged(const std::string& title, const std::string& coverUrl);

private:
    TrackChangedHandler trackChangedHandler_;
};
