# Mock Project for Cursor C++/TypeScript Workshop

This project provides a set of C++ and TypeScript files designed to demonstrate various AI-powered features of Cursor, focusing on:

*   Understanding legacy C++ code.
*   Refactoring C++ code (e.g., using smart pointers, modern STL algorithms, range-based for loops, extracting methods).
*   Translating C++ code to TypeScript.
*   Generating code and comments using AI assistance.

## Directory Structure

*   `cpp_legacy/`: Contains legacy C++ code.
    *   `include/`: Header files.
        *   `data_structures.hpp`: Defines C++ structs and classes for data handling and report generation.
    *   `src/`: Source files.
        *   `data_structures.cpp`: Implementations for `data_structures.hpp`.
        *   `utils.cpp`: Utility functions with C-style loops and raw pointer usage.
        *   `main.cpp`: A simple main function to demonstrate usage.
    *   `CMakeLists.txt`: Basic CMake file to build the C++ components.
*   `typescript_app/`: Contains the TypeScript project.
    *   `src/`: TypeScript source files.
        *   `models/`: For translated data models/interfaces.
            *   `data_models.ts`: Placeholder for TypeScript interfaces translated from C++.
        *   `services/`: For translated utility functions.
            *   `utility_functions.ts`: Placeholder for TypeScript functions translated from C++.
        *   `index.ts`: Main TypeScript file to demonstrate usage of translated code.
    *   `package.json`: NPM package configuration.
    *   `tsconfig.json`: TypeScript compiler configuration.

## Workshop Activities

Refer to your workshop outline for specific tasks. Generally:

1.  **C++ Code Understanding & Refactoring (in `cpp_legacy/`)**:
    *   Use Chat (`@-symbol`) to explain complex C++ snippets (e.g., in `utils.cpp` or `data_structures.hpp`).
    *   Use Chat for high-level refactoring strategies.
    *   Use Cmd+K to refactor C++ code (e.g., modernize loops in `utils.cpp`, convert raw pointers in `DataProcessor` to smart pointers, extract methods, add Doxygen comments to `printReport` in `data_structures.hpp`).

2.  **C++ to TypeScript Translation**:
    *   Use Chat to translate C++ classes/structs from `data_structures.hpp` to TypeScript interfaces/classes in `typescript_app/src/models/data_models.ts`.
    *   Use Chat to translate C++ functions from `utils.cpp` to TypeScript functions in `typescript_app/src/services/utility_functions.ts`.
    *   Use Cmd+K to translate smaller C++ snippets directly into `.ts` files.
    *   Observe Cursor Tab for autocomplete suggestions when manually typing translated code.

3.  **Follow-up Prompts**:
    *   Refine AI output (e.g., "ensure all public methods from the C++ class have corresponding JSDoc comments"). 