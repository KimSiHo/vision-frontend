#include <cstdlib>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickItem>
#include <gst/gst.h>
#include <spdlog/spdlog.h>
#include <vision_common/constants.hpp>
#include <vision_common/logging.hpp>

#include "controllers/command_controller.hpp"
#include "controllers/vision_controller.hpp"
#include "controllers/camera_controller.hpp"
#include "config/app_config.hpp"
#include <spdlog/cfg/env.h>  // load_env_levels

int main(int argc, char *argv[]) {
    VisionCommon::init_logging("/var/log/vision/front.log");
    spdlog::cfg::load_env_levels();  // 여기서 SPDLOG_LEVEL 읽어서 적용


    spdlog::info("Application started. Current log level: {}", spdlog::level::to_string_view(spdlog::get_level()));

    // --- 실제 애플리케이션 코드 ---
    spdlog::debug("This is a debug message.");
    spdlog::info("This is an info message.");
    spdlog::warn("This is a warning message.");

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

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        spdlog::error("Failed to load QML file.");
        return -1;
    }

//    DetectionModel detectionModel;
//    QObject::connect(visionController, &VisionController::detectionsReceived,
//                     detectionModel,   &DetectionModel::processInferenceJson,
//                     Qt::QueuedConnection);

    int ret = app.exec();

    gst_deinit();

    return ret;
}



//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlContext>
//
//#include "ai_service.h"
//#include "detection_model.h"
//
//int main(int argc, char *argv[]) {
//    QGuiApplication app(argc, argv);
//    QQmlApplicationEngine engine;
//
//    AiService service;
//    DetectionModel model;
//
//    // AiService의 신호를 DetectionModel의 슬롯에 연결
//    QObject::connect(&service, &AiService::inferenceJsonReady,
//                     &model, &DetectionModel::processInferenceJson);
//
//    // QML에서 "detectionModel"이라는 이름으로 모델을 사용할 수 있도록 등록
//    engine.rootContext()->setContextProperty("detectionModel", &model);
//
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;
//
//    // AI 서비스의 데이터 수신 시작
//    service.startListening();
//
//    return app.exec();
//}