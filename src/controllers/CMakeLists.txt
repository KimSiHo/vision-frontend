add_library(controllers
    command_controller.cpp
    vision_controller.cpp
    camera_controller.cpp
    network_controller.cpp
    capture_controller.cpp
)

target_include_directories(controllers
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
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
