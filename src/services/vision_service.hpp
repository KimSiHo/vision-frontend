#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "vision_common/constants.hpp"
#include "vision_common/sub_socket.hpp"

class VisionService {
 public:
  explicit VisionService(zmq::context_t& ctx);
  ~VisionService();

  void Start();
  void Stop();

 private:
  void Run();

  std::atomic<bool> running_{false};
  std::thread worker_;
  SubSocket sub_;  // vision_common 래퍼
};
