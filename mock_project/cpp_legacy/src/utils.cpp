#include <iostream>
#include <vector>
#include <numeric> // For std::accumulate if needed for refactoring

// Dummy resource structure for demonstration
struct MyResource {
    int id;
    std::string data;

    MyResource(int i, const std::string& d) : id(i), data(d) {
        std::cout << "MyResource " << id << " created with data: " << data << std::endl;
    }

    ~MyResource() {
        std::cout << "MyResource " << id << " destroyed." << std::endl;
    }

    void process() {
        std::cout << "Processing MyResource " << id << std::endl;
    }
};

// Function using a traditional C-style for-loop (target for modernization)
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

// Functions demonstrating raw pointer usage for resource management (target for smart pointers)
MyResource* createResource(int id, const std::string& data) {
    return new MyResource(id, data); // Potential memory leak if not managed
}

void releaseResource(MyResource* res) {
    delete res; // Caller must remember to call this
}

// Complex function for AI explanation and potential refactoring (e.g., extract method)
double calculateMetrics(const std::vector<double>& data_points) {
    if (data_points.empty()) {
        std::cout << "Cannot calculate metrics: data_points vector is empty." << std::endl;
        return 0.0;
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
            adjustment_factor += (data_points[i] - mean) * 0.1; // Arbitrary weighting
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
    int_fast16_t baseCode = 100;
    bool errorCondition = true; // Simulate some condition
    bool criticalError = false; // Simulate another condition

    if (errorCondition) {
        baseCode += 50;
        if (criticalError) {
            baseCode += 100; // Critical errors add more
        }
    }
    // Some other arbitrary logic
    for (int i=0; i < 3; ++i) {
        baseCode += i * 2;
    }
    return baseCode; // e.g. 100, 150, 250, etc.
} 