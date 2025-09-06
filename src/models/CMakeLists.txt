add_library(models STATIC
    src/detection_model.cpp
    src/bluetooth_device_model.cpp

    inc/models/bluetooth_device_model.hpp
    inc/models/detection_model.hpp
    inc/models/detection_item.hpp
)

target_include_directories(models
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(models
    PUBLIC
        qt_common
        nlohmann_json::nlohmann_json
)
