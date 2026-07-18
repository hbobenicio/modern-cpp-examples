cmake_minimum_required(VERSION 3.30)
project(cpp-server LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps/spdlog.cmake")

add_executable(cpp-server
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/strutils/strutils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/os/posix/posix.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/net/http/request-parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/net/http/buffered-reader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/main.cpp"
)

target_compile_features(cpp-server PRIVATE cxx_std_23)

target_include_directories(cpp-server PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

target_link_libraries(cpp-server PRIVATE spdlog::spdlog)
