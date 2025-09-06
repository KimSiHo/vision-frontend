#include "controllers/features/capture_controller.hpp"

#include <QDateTime>

CaptureController::CaptureController(QObject* parent) : QObject(parent) {}

void CaptureController::saveFrame(const QImage& image) {
  if (image.isNull()) return;

  QString filename = QString("/tmp/capture_%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

  image.save(filename);
}
