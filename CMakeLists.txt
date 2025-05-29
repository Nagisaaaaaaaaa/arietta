cmake_minimum_required(VERSION 3.25.2)
project(Arietta LANGUAGES CXX)

option(ARIETTA_ENABLE_TESTING "Build Arietta testing suite" ON)
option(ARIETTA_ENABLE_DEPENDENCIES "Automatically download dependency libraries if they are not present" ON)

if(ARIETTA_ENABLE_DEPENDENCIES AND NOT COMMAND CPMAddPackage)
  if(NOT DEFINED ENV{CPM_SOURCE_CACHE})
    message(FATAL_ERROR "Environment variable `CPM_SOURCE_CACHE` is not defined")
  endif()

  include(FetchContent)
  FetchContent_Declare(
    CPM
    GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
    GIT_TAG v0.42.0
  )
  FetchContent_MakeAvailable(CPM)
endif()

if(ARIETTA_ENABLE_DEPENDENCIES AND NOT TARGET Boost::ut)
  CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.3.1
  )
endif()

#
#
#
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

if(ARIETTA_ENABLE_TESTING)
  add_subdirectory(tests)
endif()
