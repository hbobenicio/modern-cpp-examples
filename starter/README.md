# starter

A simple project that tries to be a simple skeleton for well organized and modern C++ applications (with dependency management and modern cmake).

## Setup

### Pré-build dependencies

- C++ compiler with c++14 support (gcc, clang)
- cmake
- git (for using vcpkg)
- libgtkmm-3.0-dev (gtkmm vcpkg package is not trivial to build atm)

> TODO Add Ubuntu commands to install them

### vcpkg dependency manager setup

```
./scripts/vcpkg-setup.h
```

## Build

```
# TODO How to install all dependencies from the project at once?
CC=clang CXX=clang++ ./vcpkg/bootstrap-vcpkg.sh

# TODO Which version?
./vcpkg/vcpkg install spdlog 

mkdir build
cd build

# Debug
# CC=clang CXX=clang++ cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug

# Release
CC=clang CXX=clang++ cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release

make
```

## Run

```
cd build/starter-gui
./starter-gui
```
