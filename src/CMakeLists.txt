set(SRC_APP
    app/main.cpp
    controllers/command_controller.cpp
    controllers/vision_controller.cpp
    controllers/camera_controller.cpp
    services/camera_service.cpp
)

qt5_add_resources(QML_RESOURCES
    ${CMAKE_SOURCE_DIR}/resources/qml.qrc
)

add_executable(vision_frontend
    ${SRC_APP}
    ${QML_RESOURCES}
)

target_compile_definitions(vision_frontend PRIVATE SPDLOG_FMT_EXTERNAL=1)

target_link_libraries(vision_frontend
    PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
        Qt5::QuickControls2
        ${ZMQ_LIBRARIES}
        ${GST_LIBRARIES}
        ${GSTREAMER_LIBRARIES}
        vision_common::vision_common
        fmt::fmt
)

target_include_directories(vision_frontend
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ZMQ_INCLUDE_DIRS}
        ${GST_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        /opt/nvidia/deepstream/deepstream/sources/includes
)

link_directories(${GSTREAMER_LIBRARY_DIRS})

target_link_directories(vision_frontend PRIVATE
    /opt/nvidia/deepstream/deepstream/lib
)
