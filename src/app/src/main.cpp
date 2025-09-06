#include <gst/gst.h>

#include <QGuiApplication>
#include <csignal>

#include "app_bootstrap.hpp"
#include "common/qt/qt_config.hpp"
#include "common/utils/logging.hpp"
#include "config/app_config.hpp"

void cleanUp() {
  unlink("/tmp/cam.sock");
  SPDLOG_INFO("Clean process success");
}

void signalHandler(int signum) {
  SPDLOG_WARN("Signal {} received, cleaning up", signum);
  cleanUp();
  std::_Exit(EXIT_FAILURE);
}

void initCleanUp() {
  atexit(cleanUp);
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGHUP, signalHandler);
}

void initLogging() {
  app_common::initLogging(app_config::kLogFile);
  SPDLOG_INFO(R"(
===============================================
Application started.
Default logger level: {}
Service logger level: {}
ZMQ logger level: {}
Qt logger level: {}
===============================================
        )",
              spdlog::level::to_string_view(spdlog::default_logger()->level()),
              spdlog::level::to_string_view(spdlog::get("service")->level()),
              spdlog::level::to_string_view(spdlog::get("zmq")->level()),
              spdlog::level::to_string_view(spdlog::get("qt")->level()));
}

int main(int argc, char* argv[]) {
  initLogging();
  QtConfig::init();
  gst_init(&argc, &argv);
  initCleanUp();

  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  AppBootstrap boot(engine);
  boot.initAll();

  engine.load(QUrl(QStringLiteral("qrc:/qml/pages/app.qml")));
  if (engine.rootObjects().isEmpty()) {
    SPDLOG_ERROR("Failed to load QML file.");
    return -1;
  }

  int ret = app.exec();

  return ret;
}