cmake_minimum_required(VERSION 3.22)
project(kahan-float-project LANGUAGES CXX VERSION 0.0.1)
#
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
Include(FetchContent)

# ===================
# header only library
# -------------------
add_library(kahan-float INTERFACE)
target_include_directories(kahan-float INTERFACE include/)
#
add_subdirectory(demo)
#
add_subdirectory(tests)
