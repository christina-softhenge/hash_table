cmake_minimum_required(VERSION 3.10)

# Name of the project
project(HashTableApp)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Add include directory for headers
include_directories(Headers)

# Add all source files in Sources/ directory
file(GLOB SOURCES "Sources/*.cpp")

# Create the executable
add_executable(HashTableApp ${SOURCES})
