#include "services/camera/camera_service.hpp"

#include "common/utils/logging.hpp"

CameraService::CameraService() {
  if (!buildPipeline()) {
    SPDLOG_SERVICE_ERROR("Failed to build the pipeline.");
  }
}

CameraService::~CameraService() {
  stop();

  if (bus_thread_.joinable()) {
    bus_thread_.join();
  }

  if (bus_) {
    gst_object_unref(bus_);
  }

  if (pipeline_) {
    gst_object_unref(pipeline_);
  }

  gst_deinit();
  SPDLOG_SERVICE_INFO("CameraService cleaned up.");
}

void CameraService::start() { is_running_ = true; }

void CameraService::stop() { is_running_ = false; }

bool CameraService::buildPipeline() {
  if (!createElements()) {
    return false;
  }

  configureElements();

  if (!linkElements()) {
    return false;
  }

  bus_ = gst_element_get_bus(pipeline_);
  bus_thread_ = std::thread(&CameraService::busWatchLoop, this);

  return true;
}

bool CameraService::createElements() {
  pipeline_ = gst_pipeline_new("frontend-pipeline");

  shmsrc_ = gst_element_factory_make("shmsrc", "shared-memory-source");
  caps_src_ = gst_element_factory_make("capsfilter", "source-caps");
  nvvidconv_ = gst_element_factory_make("nvvidconv", "video-converter-1");
  caps_rgba_ = gst_element_factory_make("capsfilter", "rgba-caps");
  glupload_ = gst_element_factory_make("glupload", "opengl-uploader");
  sink_ = gst_element_factory_make("qmlglsink", "qml_sink");

  if (!pipeline_ || !shmsrc_ || !caps_src_ || !nvvidconv_ || !caps_rgba_ || !glupload_ || !sink_) {
    SPDLOG_SERVICE_ERROR("One or more elements could not be created.");
    return false;
  }

  return true;
}

void CameraService::configureElements() {
  g_object_set(shmsrc_, "socket-path", "/tmp/cam.sock", "do-timestamp", TRUE, "is-live", TRUE, nullptr);

  GstCaps* src_caps = gst_caps_from_string("video/x-raw,format=I420,width=960,height=540,framerate=14/1");
  g_object_set(caps_src_, "caps", src_caps, nullptr);
  gst_caps_unref(src_caps);

  GstCaps* rgba_caps = gst_caps_from_string("video/x-raw,format=RGBA");
  g_object_set(caps_rgba_, "caps", rgba_caps, nullptr);
  gst_caps_unref(rgba_caps);

  g_object_set(sink_, "sync", TRUE, nullptr);
}

bool CameraService::linkElements() {
  gst_bin_add_many(GST_BIN(pipeline_), shmsrc_, caps_src_, nvvidconv_, caps_rgba_, glupload_, sink_, nullptr);

  if (!gst_element_link_many(shmsrc_, caps_src_, nvvidconv_, caps_rgba_, glupload_, sink_, nullptr)) {
    SPDLOG_SERVICE_ERROR("Elements could not be linked.");
    return false;
  }

  return true;
}

void CameraService::busWatchLoop() {
  while (is_running_) {
    GstMessage* msg = gst_bus_timed_pop_filtered(bus_, 100 * GST_MSECOND,
                                                 static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    if (msg != nullptr) {
      switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
          GError* err = nullptr;
          gchar* debug_info = nullptr;
          gst_message_parse_error(msg, &err, &debug_info);
          SPDLOG_SERVICE_ERROR("Error from element {}: {}", GST_OBJECT_NAME(msg->src), err->message);
          SPDLOG_SERVICE_ERROR("Debugging info: {}", (debug_info ? debug_info : "none"));
          g_clear_error(&err);
          g_free(debug_info);
          is_running_ = false;
          break;
        }

        case GST_MESSAGE_EOS:
          SPDLOG_SERVICE_INFO("End-Of-Stream reached.");
          is_running_ = false;
          break;

        default:
          break;
      }

      gst_message_unref(msg);
    }
  }

  SPDLOG_SERVICE_INFO("Bus watch thread finished.");
}
