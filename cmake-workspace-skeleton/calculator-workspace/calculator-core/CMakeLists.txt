cmake_minimum_required(VERSION 3.15)

project(calculator-core
    VERSION 0.1.0
    DESCRIPTION "Calculator Core is the main library of the calculator"
    LANGUAGES CXX
)

set(CALCULATOR_CORE_SOURCES
    "${PROJECT_SOURCE_DIR}/src/calculator/core/parser/Parser.h"
    "${PROJECT_SOURCE_DIR}/src/calculator/core/parser/Parser.cpp"
    "${PROJECT_SOURCE_DIR}/include/calculator/core/Calculator.h"
    "${PROJECT_SOURCE_DIR}/src/calculator/core/Calculator.cpp"
)

add_library("${PROJECT_NAME}"        STATIC "${CALCULATOR_CORE_SOURCES}")
add_library("${PROJECT_NAME}-shared" SHARED "${CALCULATOR_CORE_SOURCES}")

# add_library("${PROJECT_NAME}" STATIC
#     "${PROJECT_SOURCE_DIR}/src/calculator/core/ShuntingYard.h"
#     "${PROJECT_SOURCE_DIR}/src/calculator/core/ShuntingYard.cpp"
#     "${PROJECT_SOURCE_DIR}/include/calculator/core/Calculator.h"
#     "${PROJECT_SOURCE_DIR}/src/calculator/core/Calculator.cpp"
# )
# add_library("${PROJECT_NAME}-shared" SHARED
#     "${PROJECT_SOURCE_DIR}/src/calculator/core/ShuntingYard.h"
#     "${PROJECT_SOURCE_DIR}/src/calculator/core/ShuntingYard.cpp"
#     "${PROJECT_SOURCE_DIR}/include/calculator/core/Calculator.h"
#     "${PROJECT_SOURCE_DIR}/src/calculator/core/Calculator.cpp"
# )

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_17)
target_compile_features("${PROJECT_NAME}-shared" PUBLIC cxx_std_17)

target_include_directories("${PROJECT_NAME}"
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
    PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
)

target_include_directories("${PROJECT_NAME}-shared"
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
    PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
)

set_property(TARGET "${PROJECT_NAME}" "${PROJECT_NAME}-shared"
    PROPERTY PUBLIC_HEADER
        "${PROJECT_SOURCE_DIR}/include/calculator/core/Calculator.h"
)

install(
    TARGETS "${PROJECT_NAME}" "${PROJECT_NAME}-shared"
    ARCHIVE DESTINATION /usr/local/lib/calculator
    LIBRARY DESTINATION /usr/local/lib/calculator
    PUBLIC_HEADER DESTINATION /usr/local/include/calculator/core
)
