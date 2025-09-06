#include "common/zmq/sub_socket.hpp"

#include "common/utils/logging.hpp"

SubSocket::SubSocket(zmq::context_t& ctx, std::string_view endpoint, std::string_view topic)
    : socket_(ctx, zmq::socket_type::sub) {
    try {
        socket_.connect(std::string(endpoint));
        socket_.set(zmq::sockopt::subscribe, topic);
        SPDLOG_ZMQ_INFO("SUB connected to {}, {}", endpoint, topic);
    } catch (const zmq::error_t& e) {
        SPDLOG_ZMQ_ERROR("SubSocket init failed: {}", e.what());
    }
}

std::optional<std::string> SubSocket::receivePayload() {
    try {
        zmq::message_t topicMsg;
        zmq::message_t dataMsg;

        if (!socket_.recv(topicMsg, zmq::recv_flags::none)) {
            SPDLOG_ZMQ_INFO("No topic message received");
            return std::nullopt;
        }

        if (!socket_.recv(dataMsg, zmq::recv_flags::none)) {
            SPDLOG_ZMQ_INFO("No data message received");
            return std::nullopt;
        }

        std::string payload = dataMsg.to_string();
        SPDLOG_ZMQ_INFO("Received payload: {}", payload);

        return payload;
    } catch (const zmq::error_t& e) {
        SPDLOG_ZMQ_ERROR("SubSocket receive error: {}", e.what());
        return std::nullopt;
    }
}

std::optional<std::pair<std::string, std::string>> SubSocket::receive() {
    zmq::message_t topicMsg;
    zmq::message_t dataMsg;

    if (!socket_.recv(topicMsg, zmq::recv_flags::none)) {
        SPDLOG_ZMQ_INFO("No topic message received");
        return std::nullopt;
    }

    if (!socket_.recv(dataMsg, zmq::recv_flags::none)) {
        SPDLOG_ZMQ_INFO("No data message received");
        return std::nullopt;
    }

    std::string topic(static_cast<char*>(topicMsg.data()), topicMsg.size());
    std::string data(static_cast<char*>(dataMsg.data()), dataMsg.size());

    SPDLOG_ZMQ_INFO("Received topic: [{}], data: [{}]", topic, data);

    return std::make_pair(std::move(topic), std::move(data));
}

int SubSocket::get_fd() const {
    int fd;
    size_t fd_size = sizeof(fd);
    socket_.getsockopt(ZMQ_FD, &fd, &fd_size);
    return fd;
}
