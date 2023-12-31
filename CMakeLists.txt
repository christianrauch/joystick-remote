cmake_minimum_required(VERSION 3.16)

project(joystick-remote VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(ZLIB REQUIRED)
find_package(MAVSDK REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appjoystick-remote
    main.cpp
)

qt_add_qml_module(appjoystick-remote
    URI joystick-remote
    VERSION 1.0
    QML_FILES Main.qml qmlJoypad/Joypad.qml
    SOURCES client-udp.cpp client-mavlink.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appjoystick-remote PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appjoystick-remote
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appjoystick-remote
    PRIVATE Qt6::Quick ZLIB::ZLIB MAVSDK::mavsdk
)

include(GNUInstallDirs)
install(TARGETS appjoystick-remote
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
