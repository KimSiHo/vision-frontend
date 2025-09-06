#include "controllers/command/command_controller.hpp"

#include <QDebug>

#include "common/utils/logging.hpp"

CommandController::CommandController(ReqSocket& req_socket, QObject* parent)
    : QObject(parent), req_socket_(req_socket) {}

QString CommandController::sendCommand(const QString& cmd) {
  std::string msg = cmd.toStdString();
  req_socket_.send(msg);

  if (auto reply = req_socket_.receive()) {
    return QString::fromStdString(*reply);
  }

  qCritical() << "No reply received from socket";
  return QStringLiteral("ERROR: no reply received");
}
