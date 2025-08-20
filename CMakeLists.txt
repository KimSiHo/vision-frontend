cmake_minimum_required(VERSION 3.21)
project(vision_frontend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

find_package(Threads REQUIRED)
find_package(vision_common REQUIRED)
find_package(fmt REQUIRED)
pkg_check_modules(GST REQUIRED
    gstreamer-1.0>=1.6
    gstreamer-video-1.0>=1.6
    gstreamer-plugins-base-1.0
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)
# add_subdirectory(sample)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)

# 실행파일 (src/CMakeLists.txt 안에서 add_executable(frontend_app ...) 했다고 가정)
install(TARGETS vision_frontend
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

# QML/리소스 있으면 같이
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qml/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/qml
    FILES_MATCHING PATTERN "*.qml"
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/resources
    FILES_MATCHING PATTERN "*"
)
