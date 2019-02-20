cmake_minimum_required(VERSION 3.10)
project(starter-gui)

# TODO Use vcpkg as dependency manager
# https://cmake.org/cmake/help/v3.10/module/FindPkgConfig.html
find_package(PkgConfig)

# Ubuntu: sudo apt install libgtkmm-3.0-dev
pkg_check_modules(GTKMM gtkmm-3.0)

# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect 
message("\n=== GTKMM ===")
message("GTKMM_LIBRARIES:\n${GTKMM_LIBRARIES}\n")
message("GTKMM_LIBRARY_DIRS:\n${GTKMM_LIBRARY_DIRS}\n")
message("GTKMM_INCLUDE_DIRS:\n${GTKMM_INCLUDE_DIRS}\n")

add_executable(${PROJECT_NAME}
    "src/starter/gui/main.cpp"
    "src/starter/gui/main-window/MainWindow.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${GTKMM_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/starter-core/include"
    "${PROJECT_SOURCE_DIR}/src"
)

# link_directories(
#     "${GTKMM_LIBRARY_DIRS}"
# )

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTKMM_LIBRARIES}
    spdlog::spdlog
    starter-core
)

# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
# https://cmake.org/cmake/help/v3.10/command/add_custom_command.html#command:add_custom_command
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/src/starter/gui/main-window/MainWindow.glade"
        "${CMAKE_CURRENT_BINARY_DIR}/main-window/MainWindow.glade"
)
