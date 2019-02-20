cmake_minimum_required(VERSION 3.10)
project(starter-core)

add_library(${PROJECT_NAME} STATIC
    "src/starter/core/fs/Paths.cpp"
)

add_library(${PROJECT_NAME}-shared SHARED
    "src/starter/core/fs/Paths.cpp"
)

# TODO Can this be specified at roots CMakeLists.txt?
# TODO Get rid of duplication
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_compile_features(${PROJECT_NAME}-shared PRIVATE cxx_std_14)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)
target_include_directories(${PROJECT_NAME}-shared PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    spdlog::spdlog
)

target_link_libraries(${PROJECT_NAME}-shared PRIVATE
    spdlog::spdlog
)
