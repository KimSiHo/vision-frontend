#pragma once

#include <atomic>
#include <thread>
#include <vector>

#include "common/zmq/sub_socket.hpp"
#include "services/music/music_service.hpp"

class ControlService {
public:
  explicit ControlService(SubSocket& sub_socket);

  void start();
  void stop();
  void registerMusicService(MusicService& service);

private:
  void poll();

  std::vector<class IService*> services_;
  SubSocket& sub_socket_;
  std::thread thread_;
  std::atomic<bool> running_{false};
};
