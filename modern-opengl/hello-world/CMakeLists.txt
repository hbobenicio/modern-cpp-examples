cmake_minimum_required(VERSION 3.5)
project(hello-world)

find_package(OpenGL REQUIRED)
# find_package(glfw3 3.1.2 REQUIRED)
# find_package(glfw3 3.2 REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 14)

# (use -DCMAKE_BUILD_TYPE=Release|Debug to change the build type)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")

file(GLOB HELLO_WORLD_SOURCES "src/*.cpp")

include_directories(
    "${OPENGL_INCLUDE_DIR}"
    "include/"
    "src/"
)

add_executable(hello-world
    "${HELLO_WORLD_SOURCES}"
)

target_link_libraries(hello-world
    ${OPENGL_LIBRARY}
    ${GLFW3_LIBRARY}
)
