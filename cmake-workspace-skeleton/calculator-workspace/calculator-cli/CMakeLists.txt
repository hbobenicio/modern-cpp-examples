cmake_minimum_required(VERSION 3.15)

project(calculator-cli
    VERSION 0.1.0
    DESCRIPTION "Calculator CLI is the command line application for the calculator"
    LANGUAGES CXX
)

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/src/calculator/cli/main.cpp"
)

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_17)

target_include_directories("${PROJECT_NAME}"
    PRIVATE
        "${CMAKE_SOURCE_DIR}/calculator-core/include"
        "${PROJECT_SOURCE_DIR}/src"
)

# EXCLUDE_FROM_ALL makes the 'tests' target optional, so it won't build when running just 'make'.
# You would need to run 'make tests' instead.
add_dependencies("${PROJECT_NAME}" calculator-core)
target_link_libraries("${PROJECT_NAME}" PUBLIC calculator-core)

install(TARGETS "${PROJECT_NAME}" DESTINATION /usr/local/bin)
