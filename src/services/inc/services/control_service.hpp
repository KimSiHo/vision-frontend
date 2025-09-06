#pragma once

#include <string>

#include "services/music/music_service.hpp"
#include "common/zmq/sub_socket.hpp"

class ControlService {
public:
    explicit ControlService(SubSocket& subSocket);

    void registerMusicService(MusicService& svc);
    void poll();

private:
    std::vector<class IService*> services_;
    SubSocket& subSocket_;
};
