#include "app_bootstrap.hpp"

#include <QQmlContext>

#include "config/zmq_config.hpp"
#include "controllers/bluetooth/bluetooth_controller.hpp"
#include "controllers/camera/camera_controller.hpp"
#include "controllers/command/command_controller.hpp"
#include "controllers/features/capture_controller.hpp"
#include "controllers/music/music_controller.hpp"
#include "controllers/network/network_controller.hpp"
#include "controllers/vision/vision_controller.hpp"
#include "models/bluetooth/bluetooth_device_model.hpp"
#include "models/vision/detection_model.hpp"
#include "services/camera/camera_service.hpp"
#include "services/control/control_service.hpp"
#include "services/music/music_service.hpp"

AppBootstrap::AppBootstrap(QQmlApplicationEngine& engine, QObject* parent)
    : QObject(parent),
      engine_(engine),
      bt_sub_(ctx_, app_config::kEventEndpoint, app_config::kTopicBluetooth),
      vision_sub_(ctx_, app_config::kEventEndpoint, app_config::kTopicDetections),
      control_sub_(ctx_, app_config::kEventEndpoint),
      req_(ctx_, app_config::kControlEndpoint) {}

AppBootstrap::~AppBootstrap() {
  for (QThread* thread : {bt_thread_, vision_thread_}) {
    if (!thread) continue;
    thread->quit();
    thread->wait();
    thread->deleteLater();
  }
}

void AppBootstrap::initAll() {
  initBluetooth();
  initVision();
  initCamera();
  initCommand();
  initNetwork();
  initCapture();
  initMusic();
  initControl();

  //  event_router_.setMessageHandler([this](const std::string& topic, const std::string& payload) {
  //    if (topic == "vision")
  //      vision_controller_->enqueue(payload);
  //    else if (topic == "music")
  //      music_controller_->enqueue(payload);
  //  });
}

void AppBootstrap::initBluetooth() {
  auto* bt_controller = new BluetoothController(bt_sub_);
  auto* bt_model = new BluetoothDeviceModel;

  bt_thread_ = new QThread(this);
  bt_controller->moveToThread(bt_thread_);
  bt_thread_->start();

  engine_.rootContext()->setContextProperty("bluetoothController", bt_controller);
  engine_.rootContext()->setContextProperty("bluetoothModel", bt_model);

  connect(bt_thread_, &QThread::started, bt_controller, &BluetoothController::start);
  connect(bt_controller, &BluetoothController::finished, bt_thread_, &QThread::quit);
  connect(bt_thread_, &QThread::finished, bt_controller, &QObject::deleteLater);
  connect(bt_thread_, &QThread::finished, bt_thread_, &QObject::deleteLater);
  connect(bt_controller, &BluetoothController::devicesUpdated, bt_model, &BluetoothDeviceModel::processJson,
          Qt::QueuedConnection);
}

void AppBootstrap::initVision() {
  auto* vision_controller = new VisionController(vision_sub_);
  auto* detection_model = new DetectionModel;

  vision_thread_ = new QThread(this);
  vision_controller->moveToThread(vision_thread_);
  vision_thread_->start();

  engine_.rootContext()->setContextProperty("visionController", vision_controller);
  engine_.rootContext()->setContextProperty("detectionModel", detection_model);

  connect(vision_controller, &VisionController::detectionsReceived, detection_model,
          &DetectionModel::processInferenceJson, Qt::QueuedConnection);
  connect(&engine_, &QQmlApplicationEngine::destroyed, vision_thread_, &QThread::quit);
  connect(vision_thread_, &QThread::finished, vision_controller, &QObject::deleteLater);
  connect(vision_thread_, &QThread::finished, vision_thread_, &QObject::deleteLater);
}

void AppBootstrap::initCamera() {
  auto* camera_service = new CameraService();
  camera_service->start();

  auto* camera_controller = new CameraController(&engine_, camera_service);
  engine_.rootContext()->setContextProperty("cameraController", camera_controller);

  connect(&engine_, &QQmlApplicationEngine::destroyed, camera_controller, &QObject::deleteLater);
  connect(&engine_, &QQmlApplicationEngine::destroyed, [camera_service]() { delete camera_service; });
}

void AppBootstrap::initCommand() {
  auto* command_controller = new CommandController(req_);
  engine_.rootContext()->setContextProperty("commandController", command_controller);

  connect(&engine_, &QQmlApplicationEngine::destroyed, command_controller, &QObject::deleteLater);
}

void AppBootstrap::initNetwork() {
  auto* network_controller = new NetworkController();
  engine_.rootContext()->setContextProperty("networkController", network_controller);

  connect(&engine_, &QQmlApplicationEngine::destroyed, network_controller, &QObject::deleteLater);
}

void AppBootstrap::initCapture() {
  auto* capture_controller = new CaptureController();
  engine_.rootContext()->setContextProperty("captureController", capture_controller);

  connect(&engine_, &QQmlApplicationEngine::destroyed, capture_controller, &QObject::deleteLater);
}

void AppBootstrap::initMusic() {
  if (!music_) music_ = std::make_unique<MusicService>();

  auto* music_controller = new MusicController();
  music_controller->bindToService(*music_);
  engine_.rootContext()->setContextProperty("musicController", music_controller);
  connect(&engine_, &QQmlApplicationEngine::destroyed, music_controller, &QObject::deleteLater);
}

void AppBootstrap::initControl() {
  auto* control_service = new ControlService(control_sub_);
  control_service->registerMusicService(*music_);
  control_service->start();
}