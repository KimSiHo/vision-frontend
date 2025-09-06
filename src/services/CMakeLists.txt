add_library(services
    STATIC
        src/impl/camera/camera_service.cpp
        src/impl/music/music_service.cpp
        src/impl/control/control_service.cpp

        src/adapters/music/music_service_adapter.cpp
)

target_include_directories(services
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(services
    PUBLIC
        GStreamer::Core
        GStreamer::Video
        GStreamer::PluginsBase

    PRIVATE
        common
        config
)
