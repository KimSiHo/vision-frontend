#include "common/zmq/event_router.hpp"

EventRouter::EventRouter(zmq::context_t& ctx, const std::string& endpoint) : sub_(ctx, endpoint, "") {}

EventRouter::~EventRouter() { stop(); }

void EventRouter::setMessageHandler(MessageHandler handler) { message_handler_ = std::move(handler); }

void EventRouter::start() {
  if (running_) return;
  running_ = true;
  thread_ = std::thread([this] { run(); });
}

void EventRouter::stop() {
  running_ = false;
  if (thread_.joinable()) thread_.join();
}

void EventRouter::run() {
  while (running_) {
    auto msg = sub_.receive();
    if (!msg) continue;

    const auto& [topic, payload] = *msg;
    if (message_handler_) {
      message_handler_(topic, payload);
    }
  }
}
