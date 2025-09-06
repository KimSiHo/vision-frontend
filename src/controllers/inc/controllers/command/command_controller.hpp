#pragma once

#include <QObject>
#include <QString>
#include <string>

#include "common/zmq/req_socket.hpp"

class CommandController : public QObject {
  Q_OBJECT

public:
  explicit CommandController(ReqSocket& req_socket, QObject* parent = nullptr);
  Q_INVOKABLE QString sendCommand(const QString& cmd);

private:
  ReqSocket& req_socket_;
};
