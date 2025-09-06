qt5_add_resources(QML_RESOURCES
    ${CMAKE_SOURCE_DIR}/resources/qml.qrc
)

add_executable(vision_frontend
    main.cpp
    ${QML_RESOURCES}
)

target_compile_definitions(vision_frontend
    PRIVATE
        SPDLOG_FMT_EXTERNAL=1
)

target_link_libraries(vision_frontend
    PRIVATE
        qt_common
        controllers
        models
        services
        config
        vision_common::vision_common
        ${ZMQ_LIBRARIES}
        ${GST_LIBRARIES}
        ${GSTREAMER_LIBRARIES}
        fmt::fmt
)
#SQLite::SQLite3


target_include_directories(vision_frontend
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${ZMQ_INCLUDE_DIRS}
        ${GST_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        /opt/nvidia/deepstream/deepstream/sources/includes
)

target_link_directories(vision_frontend
    PRIVATE
        /opt/nvidia/deepstream/deepstream/lib
        ${GSTREAMER_LIBRARY_DIRS}
)

install(TARGETS vision_frontend
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources/qml/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/vision-frontend/qml
    FILES_MATCHING PATTERN "*.qml"
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/vision-frontend/resources
    FILES_MATCHING PATTERN "*"
)
