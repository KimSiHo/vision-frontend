#pragma once

#include <string>

#include "services/music/music_service.hpp"
#include "adapters/i_service.hpp"

class MusicServiceAdapter : public IService {
public:
    explicit MusicServiceAdapter(MusicService& service);

    bool handle(const std::string& topic, const AppCommon::json& payload) override;

private:
    MusicService& svc_;
};
