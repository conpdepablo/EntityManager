cmake_minimum_required(VERSION 4.4)

project(EntityManager CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Combine Warnings and Sanitizer flags
set(CXX_ALL_FLAGS ${CXX_WARNING_FLAGS} ${CXX_SANITIZERS_FLAGS})

# Include function to create C++20 Modules
include(cmake/CreateModuleLib.cmake)

add_subdirectory(src)
add_subdirectory(tests)
