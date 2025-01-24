cmake_minimum_required(VERSION 3.10)

project(HashTableApp)

set(CMAKE_CXX_STANDARD 17)

# Add the hash_table directory for the source files
include_directories(hash_table)

# Add all source files from the hash_table directory
file(GLOB SOURCES "hash_table/Sources/*.cpp")

# Create the executable for the main app
add_executable(HashTableApp ${SOURCES})

# Add the googletest subdirectory (if it's in the tests folder)
add_subdirectory(Tests/googletest)

# Add test files located in the tests directory
file(GLOB TEST_SOURCES "Tests/*.cpp")

# Create the test executable
add_executable(MyTests ${TEST_SOURCES})

# Link Google Test with your test executable
target_link_libraries(MyTests gtest gtest_main)

