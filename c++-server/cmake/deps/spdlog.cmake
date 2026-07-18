FetchContent_Declare(
    spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG        v1.15.3

    # Other useful properties:
    #
    # GIT_PROGRESS   ON
    # GIT_SHALLOW ON
    # URL
    # URL_HASH "SHA256=..."
)

FetchContent_MakeAvailable(spdlog)

# Target spdlog::spdlog is now available
