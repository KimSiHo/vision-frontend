#include "common/qt/qt_config.hpp"

#include "common/utils/logging.hpp"

void QtConfig::init() { qInstallMessageHandler(QtConfig::messageHandler); }

void QtConfig::messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg) {
  switch (type) {
    case QtDebugMsg:
      SPDLOG_QT_DEBUG(msg.toStdString());
      break;
    case QtInfoMsg:
      SPDLOG_QT_INFO(msg.toStdString());
      break;
    case QtWarningMsg:
      SPDLOG_QT_WARN(msg.toStdString());
      break;
    case QtCriticalMsg:
      SPDLOG_QT_ERROR(msg.toStdString());
      break;
    case QtFatalMsg:
      SPDLOG_QT_ERROR(msg.toStdString());
      abort();
  }
}
