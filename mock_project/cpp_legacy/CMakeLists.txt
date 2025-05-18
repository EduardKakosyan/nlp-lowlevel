cmake_minimum_required(VERSION 3.10)
project(LegacyCppDemo VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(cpp_demo
    src/main.cpp
    src/utils.cpp
    src/data_structures.cpp
)

# Add include directories
target_include_directories(cpp_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optional: Add compiler flags for warnings (good practice)
if(MSVC)
    target_compile_options(cpp_demo PRIVATE /W4)
else()
    target_compile_options(cpp_demo PRIVATE -Wall -Wextra -pedantic)
endif()

# Message to display after configuration
message(STATUS "LegacyCppDemo configured. Build with 'cmake --build .'") 