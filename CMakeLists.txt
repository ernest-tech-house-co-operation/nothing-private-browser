cmake_minimum_required(VERSION 3.16)
project(NothingPrivateBrowser)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets)
qt_add_resources(RESOURCES assets/resources.qrc)
add_executable(nothing-private-browser
    src/main.cpp
    src/PrivateWindow.cpp
    src/FingerprintSpoofer.cpp
    src/IdentityGenerator.cpp
    src/Interceptor.cpp
    src/UpdateChecker.cpp
    ${RESOURCES}
)
target_link_libraries(nothing-private-browser
    Qt6::Core
    Qt6::Widgets
    Qt6::WebEngineWidgets
)