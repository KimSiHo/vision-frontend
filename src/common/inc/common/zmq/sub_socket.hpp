#pragma once

#include <string>
#include <optional>
#include <utility>

#include <zmq.hpp>

class SubSocket {
public:
    SubSocket(zmq::context_t& ctx, std::string_view endpoint, std::string_view topic = "");

    std::optional<std::string> receivePayload();
    std::optional<std::pair<std::string, std::string>> receive();
    int get_fd() const;
    void* handle() { return static_cast<void*>(socket_); }

    SubSocket(const SubSocket&) = delete;
    SubSocket& operator=(const SubSocket&) = delete;
    SubSocket(SubSocket&&) noexcept = default;
    SubSocket& operator=(SubSocket&&) noexcept = default;

private:
    zmq::socket_t socket_;
};
