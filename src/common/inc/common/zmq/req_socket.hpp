#pragma once

#include <string>
#include <optional>

#include <zmq.hpp>

class ReqSocket {
public:
    ReqSocket(zmq::context_t& ctx, const std::string_view endpoint);

    void send(const std::string& msg);
    std::optional<std::string> receive();

    ReqSocket(const ReqSocket&) = delete;
    ReqSocket& operator=(const ReqSocket&) = delete;
    ReqSocket(ReqSocket&&) noexcept = default;
    ReqSocket& operator=(ReqSocket&&) noexcept = default;

private:
    zmq::socket_t socket_;
};
