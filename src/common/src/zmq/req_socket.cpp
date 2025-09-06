#include "common/zmq/req_socket.hpp"

#include "common/utils/logging.hpp"

ReqSocket::ReqSocket(zmq::context_t& ctx, std::string_view endpoint) : socket_(ctx, zmq::socket_type::req) {
  try {
    socket_.connect(std::string(endpoint));
    SPDLOG_ZMQ_INFO("ReqSocket Connect: {}", endpoint);
  } catch (const zmq::error_t& e) {
    SPDLOG_ZMQ_ERROR("ReqSocket init failed: {}", e.what());
  }
}

void ReqSocket::send(const std::string& msg) {
  socket_.send(zmq::buffer(msg), zmq::send_flags::none);
  SPDLOG_ZMQ_INFO("ReqSocket Published: msg={}", msg);
}

std::optional<std::string> ReqSocket::receive() {
  zmq::message_t reply;
  if (socket_.recv(reply, zmq::recv_flags::none)) {
    std::string msg = reply.to_string();
    SPDLOG_ZMQ_INFO("Received message: {}", msg);
    return msg;
  }

  SPDLOG_ZMQ_INFO("No message received");
  return std::nullopt;
}
