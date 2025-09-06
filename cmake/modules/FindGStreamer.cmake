# Minimal FindGStreamer.cmake (works without IMPORTED_LOCATION)
# Usage:
#   list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
#   find_package(GStreamer REQUIRED)
#   target_link_libraries(services PRIVATE GStreamer::Video GStreamer::PluginsBase)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GST_CORE  REQUIRED gstreamer-1.0>=1.6)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0>=1.6)
pkg_check_modules(GST_BASE  REQUIRED gstreamer-plugins-base-1.0>=1.6)

# Core
add_library(GStreamer::Core INTERFACE IMPORTED)
set_target_properties(GStreamer::Core PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES   "${GST_CORE_INCLUDE_DIRS}"
        INTERFACE_COMPILE_OPTIONS       "${GST_CORE_CFLAGS_OTHER}"
        INTERFACE_LINK_LIBRARIES        "${GST_CORE_LIBRARIES}"
        INTERFACE_LINK_OPTIONS          "${GST_CORE_LDFLAGS_OTHER}"
)

# Video
add_library(GStreamer::Video INTERFACE IMPORTED)
set_target_properties(GStreamer::Video PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES   "${GST_VIDEO_INCLUDE_DIRS}"
        INTERFACE_COMPILE_OPTIONS       "${GST_VIDEO_CFLAGS_OTHER}"
        INTERFACE_LINK_LIBRARIES        "GStreamer::Core;${GST_VIDEO_LIBRARIES}"
        INTERFACE_LINK_OPTIONS          "${GST_VIDEO_LDFLAGS_OTHER}"
)

# PluginsBase
add_library(GStreamer::PluginsBase INTERFACE IMPORTED)
set_target_properties(GStreamer::PluginsBase PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES   "${GST_BASE_INCLUDE_DIRS}"
        INTERFACE_COMPILE_OPTIONS       "${GST_BASE_CFLAGS_OTHER}"
        INTERFACE_LINK_LIBRARIES        "GStreamer::Core;${GST_BASE_LIBRARIES}"
        INTERFACE_LINK_OPTIONS          "${GST_BASE_LDFLAGS_OTHER}"
)

# Legacy vars
set(GStreamer_FOUND TRUE)
set(GStreamer_INCLUDE_DIRS "${GST_CORE_INCLUDE_DIRS};${GST_VIDEO_INCLUDE_DIRS};${GST_BASE_INCLUDE_DIRS}")
set(GStreamer_LIBRARIES    "${GST_CORE_LIBRARIES};${GST_VIDEO_LIBRARIES};${GST_BASE_LIBRARIES}")

if(NOT GStreamer_FIND_QUIETLY)
    message(STATUS "Found GStreamer")
    message(STATUS "  Includes: ${GStreamer_INCLUDE_DIRS}")
    message(STATUS "  Libs:     ${GStreamer_LIBRARIES}")
endif()
