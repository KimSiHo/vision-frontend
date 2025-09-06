# FindZMQ.cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

add_library(ZMQ::libzmq UNKNOWN IMPORTED)
set_target_properties(ZMQ::libzmq PROPERTIES
        IMPORTED_LOCATION "${ZMQ_LIBRARY_DIRS}/lib${ZMQ_LIBRARIES}.so"
        INTERFACE_INCLUDE_DIRECTORIES "${ZMQ_INCLUDE_DIRS}"
)
