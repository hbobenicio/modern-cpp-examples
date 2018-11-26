# hello-rang

Simple project that demonstrates how to setup and use a simple library (like rang) with conan and CMake.

## Setup

```bash
# Create a python virtual environment for this project
python3 -m venv venv
. venv/bin/activate
pip install -U pip

# Install conan for the version of this project
pip install -r dev-requirements.txt

# Build it
mkdir build && cd build
conan install ..
cmake ..
make

# Test it
./bin/hello-rang
```

