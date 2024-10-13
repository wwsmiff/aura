cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
option(AURA_BUILD_TESTS OFF)
project(aura)
add_executable(${PROJECT_NAME})
set(AURA_SOURCES 
  src/string.c
  src/token.c
  src/state.c
  src/dfa.c
  src/set.c
  src/stack.c
  src/pda.c
)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_sources(${PROJECT_NAME} PRIVATE
  ${AURA_SOURCES}
  src/interpreter.c
  src/main.c
)

function(add_test TEST_SOURCE TEST_NAME)
  add_executable(${TEST_NAME})
  target_include_directories(${TEST_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_sources(${TEST_NAME} PRIVATE
    ${AURA_SOURCES}
    ${TEST_SOURCE}
  )
endfunction()

if(${AURA_BUILD_TESTS})
  add_test("${CMAKE_SOURCE_DIR}/tests/DFA/test_1.c" dfa_test_1)
  add_test("${CMAKE_SOURCE_DIR}/tests/DFA/test_2.c" dfa_test_2)

  add_test("${CMAKE_SOURCE_DIR}/tests/PDA/test_1.c" pda_test_1)
endif()
