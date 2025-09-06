#include "controllers/music/music_controller.hpp"

#include <QMetaObject>
#include <QPointer>

#include "common/utils/logging.hpp"
#include "services/music/music_service.hpp"

MusicController::MusicController(QObject* parent) : QObject(parent) {}

void MusicController::bindToService(MusicService& service) {
  SPDLOG_SERVICE_INFO("Binding to MusicService...");
  QPointer<MusicController> self(this);

  service.setTrackChangedHandler([self](const std::string& title, const std::string& cover_url) {
    if (!self) return;

    QMetaObject::invokeMethod(self.data(), "emitTrackChanged", Qt::QueuedConnection,
                              Q_ARG(QString, QString::fromStdString(title)),
                              Q_ARG(QString, QString::fromStdString(cover_url)));
  });
}

void MusicController::emitTrackChanged(QString title, QString cover_url) {
  SPDLOG_SERVICE_INFO("Emitting trackChanged: {} ({})", title.toStdString(), cover_url.toStdString());
  emit trackChanged(title, cover_url);
}
