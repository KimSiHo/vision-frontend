#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include "common/zmq/req_socket.hpp"
#include "common/zmq/sub_socket.hpp"
#include "services/music/music_service.hpp"

class AppBootstrap : public QObject {
  Q_OBJECT

public:
  explicit AppBootstrap(QQmlApplicationEngine& engine, QObject* parent = nullptr);
  ~AppBootstrap();

  void initAll();

private:
  void initBluetooth();
  void initVision();
  void initCamera();
  void initCommand();
  void initNetwork();
  void initCapture();
  void initMusic();
  void initControl();

  QQmlApplicationEngine& engine_;
  zmq::context_t ctx_{1};

  std::unique_ptr<MusicService> music_;

  SubSocket bt_sub_;
  SubSocket vision_sub_;
  SubSocket control_sub_;
  ReqSocket req_;

  QThread* bt_thread_ = nullptr;
  QThread* vision_thread_ = nullptr;

  Q_DISABLE_COPY_MOVE(AppBootstrap)
};