cmake_minimum_required(VERSION 3.10)
project(starter-gui)

add_executable(starter-gui
    "src/starter/gui/main.cpp"
)

target_include_directories(starter-gui PRIVATE
    "${CMAKE_SOURCE_DIR}/starter-core/include"
    "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(starter-gui starter-core)
