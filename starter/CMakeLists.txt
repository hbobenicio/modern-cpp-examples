cmake_minimum_required(VERSION 3.10)
project(starter)

# spdlog
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(starter-core)
add_subdirectory(starter-gui)
