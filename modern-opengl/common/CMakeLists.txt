cmake_minimum_required(VERSION 3.5)
project(common VERSION 0.1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/modern-opengl/common/Timer.cpp"
)

# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -pedantic -std=c++14)

# https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${GLFW3_LIBRARY}
        glfw
)
