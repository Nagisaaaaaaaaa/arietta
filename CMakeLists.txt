cmake_minimum_required(VERSION 3.25.2)
project(Arietta LANGUAGES CXX)

option(ARIETTA_ENABLE_TESTING "Build Arietta testing suite" ON)

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
