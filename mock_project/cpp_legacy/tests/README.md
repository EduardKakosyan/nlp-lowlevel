# Unit Tests for the Modernized C++ Legacy Code

This directory contains unit tests for the modernized C++ implementation of the legacy system. The tests are written using the Google Test framework.

## Test Coverage

The tests cover the following functionality:

- **User and Item Classes**: Testing of constructors and properties
- **LegacySystem Initialization and Cleanup**: Proper setup and teardown of the system
- **User Management**: Adding, removing, and retrieving users
- **Item Management**: Adding, updating, and retrieving items
- **Error Handling**: Proper error reporting
- **Data Processing**: Testing callback functions
- **File I/O**: Saving and loading system state
- **Notification System**: Testing owner notifications for item updates
- **Sorting**: Testing the item sorting functionality

## Prerequisites

- CMake 3.10 or higher
- A C++17 compliant compiler
- Internet connection (for the first build to download Google Test)

## Building and Running the Tests

### Using the Script

You can use the provided script to build and run the tests:

```bash
# Navigate to the project root
cd mock_project/cpp_legacy

# Make the script executable if needed
chmod +x run_tests.sh

# Run the script
./run_tests.sh
```

### Manual Build and Run

If you prefer to build and run the tests manually:

```bash
# Navigate to the project root
cd mock_project/cpp_legacy

# Create a build directory
mkdir -p build
cd build

# Configure and build
cmake ..
cmake --build . --target legacy_system_tests

# Run the tests
./tests/legacy_system_tests
```

## Test Results

When the tests run successfully, you should see output similar to:

```
[==========] Running 9 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 2 tests from UserTest
[ RUN      ] UserTest.Construction
[       OK ] UserTest.Construction (0 ms)
[----------] 2 tests from UserTest (0 ms total)

[----------] 2 tests from ItemTest
[ RUN      ] ItemTest.Construction
[       OK ] ItemTest.Construction (0 ms)
[----------] 2 tests from ItemTest (0 ms total)

[----------] 7 tests from LegacySystemTest
[ RUN      ] LegacySystemTest.InitializeAndCleanup
[       OK ] LegacySystemTest.InitializeAndCleanup (0 ms)
...
[----------] 7 tests from LegacySystemTest (0 ms total)

[----------] Global test environment tear-down
[==========] 9 tests from 3 test suites ran. (0 ms total)
[  PASSED  ] 9 tests.
```

If any tests fail, the output will indicate which tests failed and why. 