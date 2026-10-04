cmake_minimum_required(VERSION 3.16)
project(simplegui VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

add_subdirectory(src)
add_subdirectory(examples_shared)
add_subdirectory(examples_macos)
add_subdirectory(examples_linux)
add_subdirectory(examples_windows)
add_subdirectory(tools)

option(SIMPLEGUI_BUILD_TESTS "Build the SimpleGUI smoke tests" ON)
if(SIMPLEGUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
