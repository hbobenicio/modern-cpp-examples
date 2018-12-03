cmake_minimum_required(VERSION 3.5)
project(rover-simulator)

add_executable(${PROJECT_NAME}
    "src/rover/simulator/core/cmd/Command.cpp"
    "src/rover/simulator/core/cmd/TRNCommand.cpp"
    "src/rover/simulator/core/cmd/RCKCommand.cpp"
    "src/main.cpp"
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_14
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "src/"
)
