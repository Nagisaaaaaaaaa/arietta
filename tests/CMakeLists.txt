set(TESTS_NAME "${PROJECT_NAME}-Tests")

file(GLOB SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.cpp"
)

add_executable(${TESTS_NAME} ${SRC_FILES})

target_link_libraries(${TESTS_NAME} PRIVATE
  ${PROJECT_NAME}
  Boost::ut
)
