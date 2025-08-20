#pragma once

#include <gst/gst.h>
#include <thread>
#include <atomic>

class CameraService {
public:
    CameraService();
    ~CameraService();

    void start();
    void stop();

    GstElement* get_sink_element() { return sink_; }
    GstElement* get_pipeline() { return pipeline_; }

private:
    bool build_pipeline();
    bool create_elements();
    void configure_elements();
    bool link_elements();

    void bus_watch_loop();

    GstElement *pipeline_{nullptr}, *shmsrc_{nullptr}, *caps_src_{nullptr}, *nvvidconv_{nullptr}, *caps_rgba_{nullptr}, *glupload_{nullptr}, *sink_{nullptr};
    GstBus* bus_{nullptr};

    std::thread bus_thread_;
    std::atomic<bool> is_running_{false};
};
