#include "camera_controller.hpp"

#include <QQuickWindow>
#include <QQuickItem>
#include <spdlog/spdlog.h>

CameraController::CameraController(QQmlApplicationEngine *engine, CameraService *camera_service, QObject *parent)
    : engine_(engine), camera_service_(camera_service), QObject(parent) {}

CameraController::~CameraController() {
    stopPipeline();
}

void CameraController::startPipeline() {
    GstElement* pipeline = camera_service_->get_pipeline();
    if (!pipeline) {
        spdlog::warn("Pipeline is not existing.");
        return;
    }

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "qml_sink");
    if (!sink) {
        spdlog::error("Failed to get qmlglsink element from pipeline.");
        gst_object_unref(pipeline);
        pipeline = nullptr;
        return;
    }

    if (engine_->rootObjects().isEmpty()) {
        spdlog::error("QML root objects are not ready.");
        gst_object_unref(sink);
        gst_object_unref(pipeline);
        pipeline = nullptr;
        return;
    }

    QQuickWindow* rootObject = qobject_cast<QQuickWindow*>(engine_->rootObjects().first());
    QQuickItem* videoItem = rootObject->findChild<QQuickItem*>("video");

    if (!videoItem) {
        spdlog::error("GLVideoItem with objectName 'video' not found in QML.");
        gst_object_unref(sink);
        gst_object_unref(pipeline);
        pipeline = nullptr;
        return;
    }

    g_object_set(sink, "widget", videoItem, NULL);
    gst_object_unref(sink);

    spdlog::info("Starting GStreamer pipeline...");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void CameraController::stopPipeline() {
    GstElement* pipeline = camera_service_->get_pipeline();

    if (pipeline) {
        spdlog::info("Stopping GStreamer pipeline...");
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = nullptr;
    }
}
