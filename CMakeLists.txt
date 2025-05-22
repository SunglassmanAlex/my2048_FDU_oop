cmake_minimum_required(VERSION 3.10)
project(my2048)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(my2048 main.cpp Game2048.cpp)

target_link_libraries(my2048 sfml-graphics sfml-window sfml-system)
