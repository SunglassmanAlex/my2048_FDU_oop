cmake_minimum_required(VERSION 3.10)
project(My2048)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(My2048
    main.cpp
    Game2048.cpp
)

target_include_directories(My2048 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(My2048 sfml-graphics sfml-window sfml-system)
