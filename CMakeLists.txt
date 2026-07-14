cmake_minimum_required(VERSION 3.19)
project(ConnectPanel LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network SerialPort)

qt_standard_project_setup()

qt_add_executable(ConnectPanel
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    worker.h
    worker.cpp
    logmanager.h
    logmanager.cpp
    tcpclientmanager.h
    tcpclientmanager.cpp
    settingsmanager.h
    settingsmanager.cpp
    serialportmanager.h
    serialportmanager.cpp
)

target_link_libraries(ConnectPanel
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::SerialPort
)

install(TARGETS ConnectPanel
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ConnectPanel
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
