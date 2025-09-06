#include "app_config.hpp"

void AppConfig::qt_message_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "QML: %s\n", localMsg.constData());
    fflush(stderr);

    switch (type) {
        case QtDebugMsg:
            spdlog::debug("[Qt] {}", msg.toStdString());
            break;
        case QtInfoMsg:
            spdlog::info("[Qt] {}", msg.toStdString());
            break;
        case QtWarningMsg:
            spdlog::warn("[Qt] {}", msg.toStdString());
            break;
        case QtCriticalMsg:
            spdlog::error("[Qt] {}", msg.toStdString());
            break;
        case QtFatalMsg:
            spdlog::critical("[Qt] {}", msg.toStdString());
            abort();
    }
}
