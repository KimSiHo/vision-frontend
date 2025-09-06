#pragma once

#include <zmq.hpp>
#include <QString>
#include <QMessageLogContext>
#include <QtGlobal>
#include <spdlog/spdlog.h>

class AppConfig {
public:
    static zmq::context_t& context() {
        static zmq::context_t ctx{1};
        return ctx;
    }

    static void qt_message_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    AppConfig() = default;
};
