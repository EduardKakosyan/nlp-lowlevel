#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <cstdint> // For int_fast16_t

// Forward declaration if MyResource is used by other headers included by utils.hpp
// (Not strictly necessary here as it's defined below, but good practice if it were more complex)

/**
 * @brief Dummy resource structure for demonstration
 * 
 * This struct represents a resource with an ID and associated data.
 * It is used to demonstrate resource management in C++.
 * 
 * @note This is a placeholder for a real resource class.   
 */
struct MyResource {
    int id;
    std::string data;

    MyResource(int i, const std::string& d); // Constructor declaration
    ~MyResource(); // Destructor declaration
    void process(); // Member function declaration
};

// Function declarations
void processItems(int* items, int count);
MyResource* createResource(int id, const std::string& data);
void releaseResource(MyResource* res);
double calculateMetrics(const std::vector<double>& data_points);
int_fast16_t getLegacyStatusCode();

#endif // UTILS_HPP 