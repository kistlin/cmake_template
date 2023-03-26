SET(CMAKE_CXX_STANDARD 17)

cmake_minimum_required (VERSION 3.16)
set(PROJECT_NAME CMakeTemplate)
project (${PROJECT_NAME})

add_subdirectory(src)
include(GoogleTest)
enable_testing()
add_subdirectory(third_party/googletest)
add_subdirectory(test)