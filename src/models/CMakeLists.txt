add_library(models
    STATIC
        src/vision/detection_model.cpp
        src/bluetooth/bluetooth_device_model.cpp

        inc/models/vision/detection_model.hpp
        inc/models/vision/detection_item.hpp
        inc/models/bluetooth/bluetooth_device_model.hpp
)

target_include_directories(models
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(models
    PUBLIC
        qt_common

    PRIVATE
        common
)
