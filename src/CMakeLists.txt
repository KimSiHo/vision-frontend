add_subdirectory(controllers)
add_subdirectory(models)
add_subdirectory(services)
add_subdirectory(app)
add_subdirectory(config)
add_subdirectory(common)

add_library(qt_common INTERFACE)

target_link_libraries(qt_common
    INTERFACE
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
        Qt5::QuickControls2
        Qt5::Location
        Qt5::Positioning
)
