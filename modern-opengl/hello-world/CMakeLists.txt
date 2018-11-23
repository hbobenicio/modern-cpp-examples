cmake_minimum_required(VERSION 3.5)
project(hello-world)

find_package(OpenGL REQUIRED)
# find_package(glfw3 3.1.2 REQUIRED)
# find_package(glfw3 3.2 REQUIRED)
find_package(glfw3 REQUIRED)

message(OPENGL_LIBRARY " = " ${OPENGL_LIBRARY})
message(GLFW3_LIBRARY " = " ${GLFW3_LIBRARY})

set(CMAKE_CXX_STANDARD 14)

# (use -DCMAKE_BUILD_TYPE=Release|Debug to change the build type)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")

include_directories(
    "${OPENGL_INCLUDE_DIR}"
    "${CMAKE_SOURCE_DIR}/common/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

add_executable(hello-world
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# add_dependencies(hello-world commmon)

target_link_libraries(hello-world
    ${OPENGL_LIBRARY}
    ${GLFW3_LIBRARY}
    glfw
    common
)
