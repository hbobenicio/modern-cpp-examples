# starter

TODO description

## Setup

### Pré-build dependencies

- C++ compiler with c++14 support (gcc, clang)
- cmake
- git (for using vcpkg)

> TODO Add Ubuntu commands to install them

### vcpkg dependency manager setup

```
./scripts/vcpkg-setup.h
```

## Build

```
mkdir build
cd build

# Debug
# cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug

# Release
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release

make
```

## Run

```
cd build/starter-gui
./starter-gui
```
