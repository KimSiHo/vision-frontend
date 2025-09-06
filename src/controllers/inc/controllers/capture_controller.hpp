#pragma once

#include <QObject>
#include <QImage>

class CaptureController : public QObject {
    Q_OBJECT

public:
    explicit CaptureController(QObject *parent = nullptr);
    Q_INVOKABLE void saveFrame(const QImage &image);
};
