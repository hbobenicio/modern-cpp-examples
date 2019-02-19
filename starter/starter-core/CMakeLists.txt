cmake_minimum_required(VERSION 3.10)
project(starter-core)

add_library(starter-core STATIC
    "src/starter/core/Starter.cpp"
)

add_library(starter-core-shared SHARED
    "src/starter/core/Starter.cpp"
)

# TODO Isso pode ficar no CMakeList.txt raiz do projeto?
target_compile_features(starter-core PRIVATE cxx_std_14)
target_compile_features(starter-core-shared PRIVATE cxx_std_14)

message("[starter-core] CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("[starter-core] PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

target_include_directories(starter-core PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)
target_include_directories(starter-core-shared PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)
