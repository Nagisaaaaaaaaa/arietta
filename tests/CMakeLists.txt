if(NOT COMMAND CPMAddPackage)
  if(NOT DEFINED ENV{CPM_SOURCE_CACHE})
    message(FATAL_ERROR "Environment variable `CPM_SOURCE_CACHE` is not defined")
  endif()

  include(FetchContent)
  FetchContent_Declare(
    CPM
    GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
    GIT_TAG v0.43.1
  )
  FetchContent_MakeAvailable(CPM)
endif()

if(NOT TARGET Boost::ut)
  CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.3.1
  )
endif()

#
#
#
set(TESTS_NAME "${PROJECT_NAME}-Tests")

file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${TESTS_NAME} ${SRC_FILES})

if(MSVC)
  target_compile_options(${TESTS_NAME} PRIVATE /bigobj)
elseif(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${TESTS_NAME} PRIVATE -Wa,-mbig-obj)
elseif(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${TESTS_NAME} PRIVATE -Xclang -mbig-obj)
endif()

target_link_libraries(${TESTS_NAME} PRIVATE
  ${PROJECT_NAME}
  Boost::ut
)
