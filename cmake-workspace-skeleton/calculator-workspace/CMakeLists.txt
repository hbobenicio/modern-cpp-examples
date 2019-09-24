cmake_minimum_required(VERSION 3.15)

project(calculator
    VERSION 0.1.0
    DESCRIPTION "Calculator is a demonstration about Modern CMake workspace"
    LANGUAGES CXX
)

add_subdirectory(calculator-core)
add_subdirectory(calculator-cli)
