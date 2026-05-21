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
