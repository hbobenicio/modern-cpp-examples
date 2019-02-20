cmake_minimum_required(VERSION 3.10)
project(starter-core)

add_library(starter-core STATIC
    "src/starter/core/fs/Paths.cpp"
)

add_library(starter-core-shared SHARED
    "src/starter/core/fs/Paths.cpp"
)

# TODO Can this be specified at roots CMakeLists.txt?
# TODO Get rid of duplication
target_compile_features(starter-core PRIVATE cxx_std_14)
target_compile_features(starter-core-shared PRIVATE cxx_std_14)

target_include_directories(starter-core PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)
target_include_directories(starter-core-shared PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)
