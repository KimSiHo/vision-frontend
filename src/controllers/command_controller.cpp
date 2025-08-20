#include "command_controller.hpp"

#include <spdlog/spdlog.h>
#include <vision_common/pub_socket.hpp>
#include <vision_common/constants.hpp>

CommandController::CommandController(zmq::context_t& ctx, QObject* parent)
    : QObject(parent), req_(ctx, zmq::socket_type::req) {
    req_.connect(VisionCommon::COMMAND_ENDPOINT);
    spdlog::info("connect success");
}

QString CommandController::sendCommand(const QString& cmd) {
    std::string msg = cmd.toStdString();
    req_.send(zmq::buffer(msg), zmq::send_flags::none);

    zmq::message_t reply;
    if (req_.recv(reply, zmq::recv_flags::none)) {
        return QString::fromStdString(
            std::string(static_cast<char*>(reply.data()), reply.size()));
    }
}
