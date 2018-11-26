cmake_minimum_required(VERSION 3.5)
project(test-conan)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)

add_executable(test-conan
	"src/main.cpp"
)

target_compile_options(test-conan
	PRIVATE
		"-std=c++14"
)

target_link_libraries(test-conan
	CONAN_PKG::rang
)

