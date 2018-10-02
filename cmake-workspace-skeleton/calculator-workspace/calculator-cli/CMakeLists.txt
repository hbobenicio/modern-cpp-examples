cmake_minimum_required(VERSION 3.5)
project(calculator-cli)

file(GLOB CALCULATOR_CLI_SOURCES "src/calculator/cli/*.cpp")

include_directories(
    "${CMAKE_SOURCE_DIR}/calculator-core/include"
    "src"
)

# EXCLUDE_FROM_ALL makes the 'tests' target optional, so it won't build when running just 'make'.
# You would need to run 'make tests' instead.
add_executable(calculator_cli "${CALCULATOR_CLI_SOURCES}")
add_dependencies(calculator_cli calculator_core)
target_link_libraries(calculator_cli calculator_core)

install(TARGETS calculator_cli DESTINATION /usr/local/bin)
