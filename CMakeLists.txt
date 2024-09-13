cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
project(aura)
add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_sources(${PROJECT_NAME} PRIVATE
  src/main.c
  src/string.c
  src/state.c
  src/dfa.c
)
