add_library(config STATIC
    src/app_config.cpp
)

target_include_directories(config
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(config
    PUBLIC
        qt_common
)
