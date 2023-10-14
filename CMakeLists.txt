cmake_minimum_required (VERSION 3.16)

project (cpp-scratch)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_subdirectory(src)
include(GoogleTest)
enable_testing()
# add_subdirectory(third_party/asio)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/json)
add_subdirectory(third_party/spdlog)
add_subdirectory(test)
