#pragma once

#include <QObject>
#include <gst/gst.h>

class CameraController : public QObject {
    Q_OBJECT
public:
    explicit CameraController(QObject* parent=nullptr);
    ~CameraController();

    // QML의 GLVideoItem QObject*을 넘겨 연결
    Q_INVOKABLE bool start(void* qmlGlVideoItem);
    Q_INVOKABLE void stop();

private:
    GstElement* pipeline_ = nullptr;
    GstElement* sink_ = nullptr;
};
