#pragma once

#include <optional>
#include <string>
#include <utility>
#include <zmq.hpp>

class SubSocket {
public:
  SubSocket(zmq::context_t& ctx, std::string_view endpoint, std::string_view topic = "");

  std::optional<std::string> receivePayload();
  std::optional<std::pair<std::string, std::string>> receive();
  int getFd() const;
  void* handle() { return socket_; }

  SubSocket(const SubSocket&) = delete;
  SubSocket& operator=(const SubSocket&) = delete;
  SubSocket(SubSocket&&) noexcept = default;
  SubSocket& operator=(SubSocket&&) noexcept = default;

private:
  zmq::socket_t socket_;
};
