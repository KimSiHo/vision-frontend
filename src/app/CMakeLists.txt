qt5_add_resources(QML_RESOURCES
    ${CMAKE_SOURCE_DIR}/resources/qml.qrc
)

add_executable(vision_frontend
    src/main.cpp
    src/app_bootstrap.cpp
    ${QML_RESOURCES}
)

target_link_libraries(vision_frontend
    PRIVATE
        qt_common
        controllers
        models
        services
        config
        common
        ZMQ::libzmq
        GStreamer::Core
        GStreamer::Video
        GStreamer::PluginsBase
        fmt::fmt
)

target_include_directories(vision_frontend
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        /opt/nvidia/deepstream/deepstream/sources/includes
)

target_link_directories(vision_frontend
    PRIVATE
        /opt/nvidia/deepstream/deepstream/lib
)

set(CMAKE_INSTALL_PREFIX "/opt/vision-frontend" CACHE PATH "Install path prefix" FORCE)

install(TARGETS vision_frontend
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources/
    DESTINATION resources
)
