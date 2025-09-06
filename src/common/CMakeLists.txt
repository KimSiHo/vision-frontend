add_library(common
    STATIC
        src/zmq/sub_socket.cpp
        src/zmq/req_socket.cpp
        src/qt/qt_config.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(common
    PUBLIC
        qt_common
        ZMQ::libzmq
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)
