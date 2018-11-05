cmake_minimum_required(VERSION 3.10)
project(singleton_example CXX)
set(PROJECT_DESCRIPTION "Simple singleton example with a logger.")

# C++11 standard
set(CMAKE_CXX_STANDARD 11)

# Thread package needed
find_package(Threads)

# C++ extra flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")


# Application Binary Interface version for library/application interfacing
set(ABI_VERSION_MAJOR "1")
set(ABI_VERSION_MINOR "0")
set(ABI_VERSION_PATCH "0")
set(ABI_VERSION "${ABI_VERSION_MAJOR}.${ABI_VERSION_MINOR}.${ABI_VERSION_PATCH}")

# Location for the builds
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY usr/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY usr/bin)

# Logger library
add_library(logger SHARED include/Logger.h srcs/Logger.cpp)
set_target_properties(logger PROPERTIES VERSION ${ABI_VERSION} SOVERSION ${ABI_VERSION_MAJOR})


# Logger example executable
include_directories(include)
add_executable(main app/main.cpp)
set_target_properties(main PROPERTIES VERSION ${ABI_VERSION} SOVERSION ${ABI_VERSION_MAJOR})
target_link_libraries(main logger ${CMAKE_THREAD_LIBS_INIT})
