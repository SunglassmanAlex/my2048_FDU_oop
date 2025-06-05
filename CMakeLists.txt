cmake_minimum_required(VERSION 3.10)
project(My2048)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 C 标准（可选，根据需要调整）
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(/mnt/d/oop/Tasks/final/my2048/gif-h)

add_executable(My2048
    main.cpp
    Game2048.cpp
)

target_include_directories(My2048 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(My2048 sfml-graphics sfml-window sfml-system)