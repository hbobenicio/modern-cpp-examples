cmake_minimum_required(VERSION 3.5)
project(calculator-core)

file(GLOB LIBCALCULATOR_CORE_PUBLIC_HEADERS "include/calculator/core/*.h")
file(GLOB LIBCALCULATOR_CORE_SOURCES "src/calculator/core/*.cpp")

include_directories(
    "include"
)

add_library(calculator_core STATIC ${LIBCALCULATOR_CORE_SOURCES})
add_library(calculator_core_shared SHARED ${LIBCALCULATOR_CORE_SOURCES}) # TODO rename it?

set_target_properties(calculator_core PROPERTIES PUBLIC_HEADER "${LIBCALCULATOR_CORE_PUBLIC_HEADERS}")

# /usr/lib ?
install(
    TARGETS calculator_core calculator_core_shared
    ARCHIVE DESTINATION /usr/local/lib/calculator
    LIBRARY DESTINATION /usr/local/lib/calculator
    PUBLIC_HEADER DESTINATION /usr/local/include/calculator/core
)
