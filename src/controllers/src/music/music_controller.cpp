#include "controllers/music/music_controller.hpp"

#include <QMetaObject>
#include <QPointer>

#include "common/utils/logging.hpp"
#include "services/music/music_service.hpp"

MusicController::MusicController(QObject* parent)
    : QObject(parent) {}

void MusicController::bindToService(MusicService& service) {
    SPDLOG_SERVICE_INFO("Binding to MusicService...");
    // 약한 포인터로 자기 자신을 보관 (파괴 시 null)
    QPointer<MusicController> self(this);

    service.setTrackChangedHandler(
        [self](const std::string& title, const std::string& coverUrl) {
            // 컨트롤러가 이미 파괴되었으면 스킵
            if (!self) return;

            // 메인 스레드로 안전하게 큐잉
            QMetaObject::invokeMethod(
                self.data(),
                "emitTrackChanged",
                Qt::QueuedConnection,
                Q_ARG(QString, QString::fromStdString(title)),
                Q_ARG(QString, QString::fromStdString(coverUrl))
            );
        }
    );
}

void MusicController::emitTrackChanged(QString title, QString coverUrl) {
    SPDLOG_SERVICE_INFO("Emitting trackChanged: {} ({})", title.toStdString(), coverUrl.toStdString());
    emit trackChanged(title, coverUrl);
}
