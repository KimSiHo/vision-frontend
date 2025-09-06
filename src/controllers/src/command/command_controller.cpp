#include "controllers/command/command_controller.hpp"

#include <QDebug>

#include "common/utils/logging.hpp"

CommandController::CommandController(ReqSocket& reqSocket, QObject* parent)
    : QObject(parent),
      reqSocket_(reqSocket) { }

QString CommandController::sendCommand(const QString& cmd) {
    std::string msg = cmd.toStdString();
    reqSocket_.send(msg);

    if (auto reply = reqSocket_.receive()) {
        return QString::fromStdString(*reply);
    }

    qCritical() << "No reply received from socket";
    return QStringLiteral("ERROR: no reply received");
}
