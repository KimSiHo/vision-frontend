add_library(models
    detection_model.cpp
)

target_include_directories(models
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(models
    PUBLIC
        qt_common
)
