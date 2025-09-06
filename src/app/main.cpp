#include <cstdlib>

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

#include "command_controller.hpp"
#include "vision_controller.hpp"
#include "camera_controller.hpp"
#include "network_controller.hpp"
#include "capture_controller.hpp"
#include "detection_model.hpp"
#include "app_config.hpp"

int main(int argc, char *argv[]) {
    VisionCommon::init_logging("/var/log/vision/front.log");
    spdlog::cfg::load_env_levels();
    spdlog::info("Application started. Current log level: {}", spdlog::level::to_string_view(spdlog::get_level()));

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

    engine.load(QUrl(QStringLiteral("qrc:/qml/app.qml")));
    if (engine.rootObjects().isEmpty()) {
        spdlog::error("Failed to load QML file.");
        return -1;
    }

    int ret = app.exec();

    gst_deinit();

    return ret;
}
