cmake_minimum_required(VERSION 3.5)
project(hello-world VERSION 0.1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(hello-world
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -pedantic -std=c++14)

# https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${OPENGL_INCLUDE_DIR}"

        "${CMAKE_SOURCE_DIR}/common/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARY}
    ${GLFW3_LIBRARY}
    glfw
    common
)
