cmake_minimum_required(VERSION 3.10)
project(LegacyCppTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for downloading GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the test executable
add_executable(legacy_system_tests
  legacy_system_tests.cpp
  ${CMAKE_SOURCE_DIR}/../src/legacy_system_class.cpp
  ${CMAKE_SOURCE_DIR}/../src/data_structures.cpp
  ${CMAKE_SOURCE_DIR}/../src/utils.cpp
)

# Add include directories
target_include_directories(legacy_system_tests PRIVATE 
  ${CMAKE_SOURCE_DIR}/..
  ${CMAKE_SOURCE_DIR}/../include
)

# Link against GoogleTest
target_link_libraries(legacy_system_tests
  GTest::gtest
  GTest::gtest_main
)

# Enable testing via CTest
include(GoogleTest)
gtest_discover_tests(legacy_system_tests) 