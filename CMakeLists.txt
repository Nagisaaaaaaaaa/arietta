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

target_compile_definitions(${PROJECT_NAME} INTERFACE
  "$<$<COMPILE_LANGUAGE:CUDA>:ART_SPECIFIER=__host__ __device__>"
  "$<$<NOT:$<COMPILE_LANGUAGE:CUDA>>:ART_SPECIFIER=>"
)

if(ARIETTA_ENABLE_TESTING)
  add_subdirectory(tests)
endif()
