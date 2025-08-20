#include "camera_controller.hpp"

#include <QDebug>
#include <gst/gst.h>
#include <gst/video/video.h>

CameraController::CameraController(QObject* parent) : QObject(parent) {
    static bool gst_inited = false;
    if (!gst_inited) {
        gst_inited = true;
        gst_init(nullptr, nullptr);
    }
}

CameraController::~CameraController() { stop(); }

bool CameraController::start(void* qmlGlVideoItem) {
    stop();

    // 백엔드 caps: I420 640x480 14fps
    const char* desc =
        "shmsrc socket-path=/tmp/cam.sock is-live=true do-timestamp=true "
        "! video/x-raw,format=I420,width=640,height=480,framerate=14/1 "
        "! queue "
        "! videoconvert "
        "! qmlglsink name=s";

    GError* err = nullptr;
    pipeline_ = gst_parse_launch(desc, &err);
    if (!pipeline_) {
        qWarning() << "gst_parse_launch failed:" << (err? err->message : "");
        if (err) g_error_free(err);
        return false;
    }

    sink_ = gst_bin_get_by_name(GST_BIN(pipeline_), "s");
    if (!sink_) {
        qWarning() << "cannot find qmlglsink";
        stop();
        return false;
    }

    // qmlglsink에 GLVideoItem 연결
    g_object_set(G_OBJECT(sink_), "widget", qmlGlVideoItem, NULL);

    // 바로 재생
    gst_element_set_state(pipeline_, GST_STATE_PLAYING);
    return true;
}

void CameraController::stop() {
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_NULL);
        if (sink_) { gst_object_unref(sink_); sink_ = nullptr; }
        gst_object_unref(pipeline_); pipeline_ = nullptr;
    }
}
