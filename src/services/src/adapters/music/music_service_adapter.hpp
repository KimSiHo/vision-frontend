#pragma once

#include <string>

#include "adapters/i_service.hpp"
#include "services/music/music_service.hpp"

class MusicServiceAdapter : public IService {
public:
  explicit MusicServiceAdapter(MusicService& service);

  bool handle(const std::string& topic, const app_common::Json& payload) override;

private:
  MusicService& service_;
};
