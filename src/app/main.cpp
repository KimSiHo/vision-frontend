#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <gst/gst.h>
#include <vision_common/constants.hpp>
#include <vision_common/logging.hpp>
#include <spdlog/spdlog.h>

#include "controllers/command_controller.hpp"
#include "controllers/vision_controller.hpp"
#include "controllers/camera_controller.hpp"
#include "config/app_config.hpp"

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    VisionCommon::init_logging("/var/log/vision/front.log");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CommandController commandController(AppConfig::context());
    engine.rootContext()->setContextProperty("commandController", &commandController);

    VisionController visionController(AppConfig::context());
    engine.rootContext()->setContextProperty("visionController", &visionController);

    CameraController camera_controller;
    engine.rootContext()->setContextProperty("cameraController", &camera_controller);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject* root = engine.rootObjects().first();
    QObject* video = root->findChild<QObject*>("video");
    if (!video) {
        spdlog::error("GLVideoItem (objectName='video') not found");
    }

    return app.exec();
}
