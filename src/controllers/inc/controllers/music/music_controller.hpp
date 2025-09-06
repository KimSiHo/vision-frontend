#pragma once

#include <QObject>
#include <QString>

#include "services/music/music_service.hpp"

class MusicController : public QObject {
  Q_OBJECT

public:
  explicit MusicController(QObject* parent = nullptr);
  void bindToService(MusicService& service);

signals:
  void trackChanged(QString title, QString cover_url);

private slots:
  void emitTrackChanged(QString title, QString cover_url);
};
