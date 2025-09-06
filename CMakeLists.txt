cmake_minimum_required(VERSION 3.21)
project(vision_frontend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)
#Location Positioning
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Threads REQUIRED)
find_package(vision_common REQUIRED)
find_package(fmt REQUIRED)
#find_package(SQLite3 REQUIRED)

pkg_check_modules(ZMQ REQUIRED libzmq)
pkg_check_modules(GST REQUIRED
    gstreamer-1.0>=1.6
    gstreamer-video-1.0>=1.6
    gstreamer-plugins-base-1.0
)

add_subdirectory(src)

include(GNUInstallDirs)
