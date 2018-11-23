cmake_minimum_required(VERSION 3.5)
project(common)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(common
    "src/modern-opengl/common/Timer.cpp"
)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/local/include/modern-opengl/common/include
    PRIVATE src)

include_directories(
    "${OPENGL_INCLUDE_DIR}"
    "include/"
    "src/"
)

target_link_libraries(common
    ${OPENGL_LIBRARY}
    ${GLFW3_LIBRARY}
    glfw
)
