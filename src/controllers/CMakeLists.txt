add_library(controllers STATIC
    src/command_controller.cpp
    src/vision_controller.cpp
    src/camera_controller.cpp
    src/network_controller.cpp
    src/capture_controller.cpp
    src/bluetooth_controller.cpp

    inc/controllers/camera_controller.hpp
    inc/controllers/capture_controller.hpp
    inc/controllers/command_controller.hpp
    inc/controllers/network_controller.hpp
    inc/controllers/vision_controller.hpp
    inc/controllers/bluetooth_controller.hpp
)

target_compile_options(controllers
        PRIVATE
        -Wno-deprecated-declarations
)

target_include_directories(controllers
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${ZMQ_INCLUDE_DIRS}
        ${GST_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
)

target_link_libraries(controllers
    PUBLIC
        qt_common
        services
        config
        vision_common::vision_common
        ${ZMQ_LIBRARIES}
        ${GST_LIBRARIES}
        ${GSTREAMER_LIBRARIES}
)
