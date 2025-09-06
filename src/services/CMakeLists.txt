add_library(services
    camera_service.cpp
)

target_include_directories(services
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GST_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
)

target_link_libraries(services
    PUBLIC
        ${GST_LIBRARIES}
        ${GSTREAMER_LIBRARIES}
)
