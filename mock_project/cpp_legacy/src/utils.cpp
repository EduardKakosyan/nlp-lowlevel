#include "../include/utils.hpp"
#include <iostream>
#include <vector>
#include <numeric> 

namespace {
    const double DEFAULT_METRIC_VALUE = 0.0;
    const double WEIGHTING_FACTOR = 0.1;
    const int_fast16_t BASE_STATUS_CODE = 100;
    const int_fast16_t ERROR_STATUS_INCREMENT = 50;
    const int_fast16_t CRITICAL_ERROR_STATUS_INCREMENT = 100;
    const int LEGACY_STATUS_LOOP_LIMIT = 3;
    const int LEGACY_STATUS_LOOP_MULTIPLIER = 2;
}

MyResource::MyResource(int i, const std::string& d) : id(i), data(d) {
    std::cout << "MyResource " << id << " created with data: " << data << std::endl;
}

MyResource::~MyResource() {
    std::cout << "MyResource " << id << " destroyed." << std::endl;
}

void MyResource::process() {
    std::cout << "Processing MyResource " << id << std::endl;
}

/**
 * @brief Processes an array of items using a traditional C-style for-loop
 * 
 * This function takes a pointer to an array of integers and the number of elements
 * in the array. It processes each item by adding 5 to it and prints the result.
 * 
 * @param items Pointer to the array of integers to be processed
 * @param count Number of elements in the array
 * 
 * @note This function assumes that the array is valid and that the number of elements is positive.
 */
void processItems(int* items, int count) {
    std::cout << "Processing items using C-style for-loop:" << std::endl;
    if (!items || count <= 0) {
        std::cout << "No items to process or invalid arguments." << std::endl;
        return;
    }
    for (int i = 0; i < count; ++i) {
        items[i] += 5; // Example processing
        std::cout << "Item " << i << " processed to: " << items[i] << std::endl;
    }
}

/**
 * @brief Creates a new MyResource object
 * 
 * This function creates a new MyResource object with the given ID and data.
 * 
 * @param id The ID of the resource
 * @param data The data associated with the resource
 * 
 * @return A pointer to the new MyResource object
 * 
 * @note This function creates a new MyResource object and returns a pointer to it.
 *       The caller is responsible for managing the memory of the returned object.
 */
MyResource* createResource(int id, const std::string& data) {
    return new MyResource(id, data); // Potential memory leak if not managed
}

/**
 * @brief Releases a MyResource object
 * 
 * This function releases a MyResource object by deleting it.
 * 
 * @param res Pointer to the MyResource object to be released
 * 
 * @note This function deletes the MyResource object and releases the memory.
 */
void releaseResource(MyResource* res) {
    delete res; // Caller must remember to call this
}

// Complex function for AI explanation and potential refactoring (e.g., extract method)
double calculateMetrics(const std::vector<double>& data_points) {
    if (data_points.empty()) {
        std::cout << "Cannot calculate metrics: data_points vector is empty." << std::endl;
        return DEFAULT_METRIC_VALUE;
    }

    double sum = 0.0;
    for (size_t i = 0; i < data_points.size(); ++i) {
        sum += data_points[i];
    }
    double mean = sum / data_points.size();

    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < data_points.size(); ++i) {
        double diff = data_points[i] - mean;
        sum_sq_diff += diff * diff;
    }
    double variance = sum_sq_diff / data_points.size();

    // Section candidate for 'Extract Method'
    // Calculate a weighted adjustment for values above the mean
    double adjustment_factor = 0.0;
    int count_above_mean = 0;
    for(size_t i = 0; i < data_points.size(); ++i) {
        if (data_points[i] > mean) {
            adjustment_factor += (data_points[i] - mean) * WEIGHTING_FACTOR;
            count_above_mean++;
        }
    }
    if (count_above_mean > 0) {
        adjustment_factor /= count_above_mean;
    }

    std::cout << "Calculated Mean: " << mean << ", Variance: " << variance << ", Adjustment: " << adjustment_factor << std::endl;
    return mean + variance + adjustment_factor; // Arbitrary combined metric
}

// A simple utility function that could be part of a larger class or namespace
int_fast16_t getLegacyStatusCode() {
    // This function simulates fetching a status code from an older system
    // It involves multiple steps that could be simplified or explained
    int_fast16_t baseCode = BASE_STATUS_CODE;
    bool errorCondition = true; // Simulate some condition
    bool criticalError = false; // Simulate another condition

    if (errorCondition) {
        baseCode += ERROR_STATUS_INCREMENT;
        if (criticalError) {
            baseCode += CRITICAL_ERROR_STATUS_INCREMENT;
        }
    }
    // Some other arbitrary logic
    for (int i=0; i < LEGACY_STATUS_LOOP_LIMIT; ++i) {
        baseCode += i * LEGACY_STATUS_LOOP_MULTIPLIER;
    }
    return baseCode; // e.g. 100, 150, 250, etc.
} 