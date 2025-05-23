#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure and build
cmake ..
cmake --build . --target legacy_system_tests

# Run tests with detailed output
./tests/legacy_system_tests --gtest_color=yes 