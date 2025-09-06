#include <csignal>
#include <thread>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controllers/camera/camera_controller.hpp"
#include "config/app_config.hpp"
#include "config/zmq_config.hpp"
#include "common/qt/qt_config.hpp"
#include "common/utils/logging.hpp"
#include "services/control_service.hpp"
#include "app_init.hpp"

void cleanup() {
    unlink("/tmp/cam.sock");
    SPDLOG_INFO("clean process success");
}

void signalHandler(int signum) {
    SPDLOG_WARN("Signal {} received, cleaning up", signum);
    cleanup();
    std::_Exit(EXIT_FAILURE);
}

void init_logging() {
    AppCommon::initLogging(AppConfig::LOG_FILE);
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
        spdlog::level::to_string_view(spdlog::get("qt")->level())
    );
}

int main(int argc, char *argv[]) {
    init_logging();

    atexit(cleanup);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGHUP, signalHandler);

    QtConfig::init();
    gst_init(&argc, &argv);

    zmq::context_t ctx{1};

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CameraService cameraService;
    cameraService.start();

    CameraController cameraController(&engine, &cameraService);
    engine.rootContext()->setContextProperty("cameraController", &cameraController);

    AppInit::initCommand(engine, ctx);

    AppInit::initVision(engine, ctx);

    AppInit::initNetwork(engine);

    AppInit::initCapture(engine);

    AppInit::initBluetooth(engine, ctx);

    MusicService music;
    AppInit::initMusic(engine, music);

    SubSocket controlSubSocket(ctx, AppConfig::EVENT_ENDPOINT);

    ControlService control(controlSubSocket);
    control.registerMusicService(music);

    std::thread pollThread([&]() {
        control.poll();
    });
    pollThread.detach();

    engine.load(QUrl(QStringLiteral("qrc:/qml/pages/app.qml")));
    if (engine.rootObjects().isEmpty()) {
        SPDLOG_ERROR("Failed to load QML file.");
        return -1;
    }

    int ret = app.exec();

    return ret;
}
