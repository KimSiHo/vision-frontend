#pragma once

#include <QQmlApplicationEngine>
#include <QThread>

#include "controllers/bluetooth/bluetooth_controller.hpp"
#include "controllers/command/command_controller.hpp"
#include "controllers/vision/vision_controller.hpp"
#include "controllers/network/network_controller.hpp"
#include "controllers/features/capture_controller.hpp"
#include "controllers/music/music_controller.hpp"
#include "services/music/music_service.hpp"
#include "models/bluetooth/bluetooth_device_model.hpp"
#include "models/vision/detection_model.hpp"
#include "common/zmq/req_socket.hpp"

namespace AppInit {
    inline void initBluetooth(QQmlApplicationEngine& engine, zmq::context_t& ctx) {
        static SubSocket blueSubSocket(ctx, AppConfig::EVENT_ENDPOINT, AppConfig::TOPIC_BLUETOOTH);

        auto btController = new BluetoothController(blueSubSocket);
        QThread* thread = new QThread;

        btController->moveToThread(thread);

        QObject::connect(thread, &QThread::started, btController, &BluetoothController::start);
        QObject::connect(btController, &BluetoothController::finished, thread, &QThread::quit);
        QObject::connect(thread, &QThread::finished, btController, &QObject::deleteLater);
        QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        thread->start();

        engine.rootContext()->setContextProperty("bluetoothController", btController);

        auto btModel = new BluetoothDeviceModel;
        engine.rootContext()->setContextProperty("bluetoothModel", btModel);

        QObject::connect(btController, &BluetoothController::devicesUpdated,
                         btModel, &BluetoothDeviceModel::processJson,
                         Qt::QueuedConnection);
    }

    inline void initVision(QQmlApplicationEngine& engine, zmq::context_t& ctx) {
        static SubSocket visionSubSocket(ctx, AppConfig::EVENT_ENDPOINT, AppConfig::TOPIC_DETECTIONS);

        static VisionController* visionController = new VisionController(visionSubSocket);
        static DetectionModel* detectionModel = new DetectionModel();

        // QML 바인딩 (UI 스레드에서만)
        engine.rootContext()->setContextProperty("visionController", visionController);
        engine.rootContext()->setContextProperty("detectionModel", detectionModel);

        // 별도 스레드 생성
        static QThread* visionThread = new QThread();
        visionController->moveToThread(visionThread);
        visionThread->start();

        QObject::connect(visionController, &VisionController::detectionsReceived,
                         detectionModel, &DetectionModel::processInferenceJson,
                         Qt::QueuedConnection);

        QObject::connect(&engine, &QQmlApplicationEngine::destroyed,
                         visionThread, &QThread::quit);
    }

    inline void initCommand(QQmlApplicationEngine& engine, zmq::context_t& ctx) {
        static ReqSocket reqSocket(ctx, AppConfig::CONTROL_ENDPOINT);
        static CommandController commandController(reqSocket);

        engine.rootContext()->setContextProperty("commandController", &commandController);
    }

    inline void initNetwork(QQmlApplicationEngine& engine) {
        static NetworkController netController;

        engine.rootContext()->setContextProperty("networkController", &netController);
    }

    inline void initCapture(QQmlApplicationEngine& engine) {
        static CaptureController captureController;

        engine.rootContext()->setContextProperty("captureController", &captureController);
    }

    inline void initMusic(QQmlApplicationEngine& engine, MusicService& music) {
        static MusicController musicController;
        musicController.bindToService(music);
        engine.rootContext()->setContextProperty("musicController", &musicController);
    }
}
