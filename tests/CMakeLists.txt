set(TESTS_NAME "${PROJECT_NAME}-Tests")

file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${TESTS_NAME} ${SRC_FILES})

target_link_libraries(${TESTS_NAME} PRIVATE
  ${PROJECT_NAME}
  Boost::ut
)
