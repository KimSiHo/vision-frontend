#pragma once

#include <atomic>
#include <functional>
#include <string>
#include <thread>

#include "common/zmq/sub_socket.hpp"

class EventRouter {
public:
  using MessageHandler = std::function<void(const std::string& topic, const std::string& payload)>;

  EventRouter(zmq::context_t& ctx, const std::string& endpoint);
  ~EventRouter();

  void setMessageHandler(MessageHandler handler);

  void start();
  void stop();

private:
  void run();

  SubSocket sub_;
  std::thread thread_;
  std::atomic_bool running_{false};
  MessageHandler message_handler_;
};
