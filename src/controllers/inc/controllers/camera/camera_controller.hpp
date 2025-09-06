#pragma once

#include <gst/gst.h>

#include <QObject>
#include <QQmlApplicationEngine>

#include "services/camera/camera_service.hpp"

class CameraController : public QObject {
  Q_OBJECT

public:
  explicit CameraController(QQmlApplicationEngine* engine, CameraService* camera_service, QObject* parent = nullptr);
  ~CameraController();

  Q_INVOKABLE void startPipeline();
  Q_INVOKABLE void stopPipeline();

private:
  QQmlApplicationEngine* engine_{nullptr};
  CameraService* camera_service_{nullptr};
};
