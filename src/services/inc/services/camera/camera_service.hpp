#pragma once

#include <gst/gst.h>

#include <atomic>
#include <thread>

class CameraService {
public:
  CameraService();
  ~CameraService();

  void start();
  void stop();

  GstElement* getSinkElement() { return sink_; }
  GstElement* getPipeline() { return pipeline_; }

private:
  bool buildPipeline();
  bool createElements();
  void configureElements();
  bool linkElements();
  void busWatchLoop();

  GstElement* pipeline_{nullptr};
  GstElement* shmsrc_{nullptr};
  GstElement* caps_src_{nullptr};
  GstElement* nvvidconv_{nullptr};
  GstElement* caps_rgba_{nullptr};
  GstElement* glupload_{nullptr};
  GstElement* sink_{nullptr};
  GstBus* bus_{nullptr};

  std::thread bus_thread_;
  std::atomic<bool> is_running_{false};
};
