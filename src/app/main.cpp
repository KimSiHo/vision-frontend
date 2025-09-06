#include <cstdlib>
#include <iostream>
#include <csignal>
#include <unistd.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickItem>
#include <gst/gst.h>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <vision_common/constants.hpp>
#include <vision_common/logging.hpp>

#include "controllers/command_controller.hpp"
#include "controllers/vision_controller.hpp"
#include "controllers/camera_controller.hpp"
#include "controllers/network_controller.hpp"
#include "controllers/capture_controller.hpp"
#include "controllers/bluetooth_controller.hpp"
#include "models/bluetooth_device_model.hpp"
#include "models/detection_model.hpp"
#include "config/app_config.hpp"

void cleanup() {
    unlink("/tmp/cam.sock");
    spdlog::info("delete success");
}

void signalHandler(int signum) {
    spdlog::warn("Signal {} received, cleaning up...", signum);
    cleanup();
    std::_Exit(EXIT_FAILURE);  // 안전하게 종료
}

int main(int argc, char *argv[]) {
    VisionCommon::init_logging("/var/log/vision/front.log");
    spdlog::cfg::load_env_levels();
    spdlog::info("Application started. Current log level: {}", spdlog::level::to_string_view(spdlog::get_level()));

    atexit(cleanup);
    std::signal(SIGINT, signalHandler);   // Ctrl+C
    std::signal(SIGTERM, signalHandler);  // kill
    std::signal(SIGHUP, signalHandler);   // 터미널 끊김 등

    qInstallMessageHandler(AppConfig::qt_message_handler);

    gst_init(&argc, &argv);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CameraService cameraService;
    cameraService.start();

    CameraController cameraController(&engine, &cameraService);
    engine.rootContext()->setContextProperty("cameraController", &cameraController);

    CommandController commandController(AppConfig::context());
    engine.rootContext()->setContextProperty("commandController", &commandController);

    VisionController visionController(AppConfig::context());
    engine.rootContext()->setContextProperty("visionController", &visionController);

    DetectionModel detectionModel;
    engine.rootContext()->setContextProperty("detectionModel", &detectionModel);

    NetworkController netController;
    engine.rootContext()->setContextProperty("networkController", &netController);

    CaptureController captureController;
    engine.rootContext()->setContextProperty("captureController", &captureController);

    QObject::connect(&visionController, &VisionController::detectionsReceived,
                     &detectionModel, &DetectionModel::processInferenceJson,
                     Qt::QueuedConnection);

    BluetoothController btController(AppConfig::context());
    engine.rootContext()->setContextProperty("bluetoothController", &btController);

    BluetoothDeviceModel btModel;
    engine.rootContext()->setContextProperty("bluetoothModel", &btModel);

    bool ok = QObject::connect(&btController, &BluetoothController::devicesUpdated,
                     &btModel, &BluetoothDeviceModel::processJson,
                     Qt::QueuedConnection);

    if (!ok) spdlog::error("Failed to connect btController -> btModel");

    engine.load(QUrl(QStringLiteral("qrc:/qml/app.qml")));
    if (engine.rootObjects().isEmpty()) {
        spdlog::error("Failed to load QML file.");
        return -1;
    }

    int ret = app.exec();

    gst_deinit();

    return ret;
}
