# Arietta

A collection of C++ template metaprogramming utilities and experiments.

## Getting Started

Use [CMake](https://cmake.org/) and [CPM](https://github.com/cpm-cmake/CPM.cmake) to add this library to your project.

```cmake
CPMAddPackage(
  NAME arietta
  GITHUB_REPOSITORY Nagisaaaaaaaaa/arietta
  GIT_TAG main
  OPTIONS "ARIETTA_ENABLE_TESTING OFF"
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
  Arietta
)
```
