#pragma once
#include <QObject>
#include <QImage>

class CaptureController : public QObject {
    Q_OBJECT   // ✅ 꼭 있어야 moc가 작동함
public:
    explicit CaptureController(QObject *parent = nullptr);
    Q_INVOKABLE void saveFrame(const QImage &image);
};
