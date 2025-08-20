#pragma once

#include <string>

#include <QObject>
#include <QString>
#include <zmq.hpp>

class CommandController : public QObject {
    Q_OBJECT
public:
    explicit CommandController(zmq::context_t& ctx, QObject* parent = nullptr);
    Q_INVOKABLE QString sendCommand(const QString& cmd);

private:
    zmq::socket_t req_;
};
