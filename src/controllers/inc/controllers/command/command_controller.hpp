#pragma once

#include <string>

#include <QObject>
#include <QString>

#include "common/zmq/req_socket.hpp"

class CommandController : public QObject {
    Q_OBJECT

public:
    explicit CommandController(ReqSocket& reqSocket, QObject* parent = nullptr);
    Q_INVOKABLE QString sendCommand(const QString& cmd);

private:
    ReqSocket& reqSocket_;
};
