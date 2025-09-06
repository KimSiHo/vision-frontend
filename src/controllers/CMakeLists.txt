add_library(controllers
    STATIC
        src/camera/camera_controller.cpp
        src/features/capture_controller.cpp
        src/command/command_controller.cpp
        src/network/network_controller.cpp
        src/vision/vision_controller.cpp
        src/bluetooth/bluetooth_controller.cpp
        src/music/music_controller.cpp

        inc/controllers/camera/camera_controller.hpp
        inc/controllers/features/capture_controller.hpp
        inc/controllers/command/command_controller.hpp
        inc/controllers/network/network_controller.hpp
        inc/controllers/vision/vision_controller.hpp
        inc/controllers/bluetooth/bluetooth_controller.hpp
        inc/controllers/music/music_controller.hpp
)

target_include_directories(controllers
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(controllers
    PUBLIC
        common
        ZMQ::libzmq
        GStreamer::Core
        GStreamer::Video
        GStreamer::PluginsBase

    PRIVATE
        qt_common
        services
        config
)
