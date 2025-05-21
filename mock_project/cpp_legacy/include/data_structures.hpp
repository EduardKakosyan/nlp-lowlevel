#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream> // For demonstration purposes

// A simple C-style struct for demo
struct LegacyPoint {
    int x;
    int y;
};

// Class with a raw pointer member and C-style loop for processing
class DataProcessor {
public:
    // Consider making this a static const or constexpr if C++17 allows inline initialization for it.
    // For now, as a literal, it's clear. If it had more semantic meaning, a const would be good.
    // Let's add it as a static constexpr member.
    static constexpr int PROCESSING_MULTIPLIER = 2;

    int* raw_data_array;
    int size;

    DataProcessor(int* data, int s) : raw_data_array(new int[s]), size(s) {
        // Simulate initialization, vulnerable to issues if data is smaller than s
        for (int i = 0; i < s; ++i) {
            if (data) { // Basic null check
                 raw_data_array[i] = data[i];
            } else {
                 raw_data_array[i] = 0; // Default init
            }
        }
        std::cout << "DataProcessor created with data." << std::endl;
    }

    ~DataProcessor() {
        delete[] raw_data_array;
        std::cout << "DataProcessor destroyed, memory freed." << std::endl;
    }

    void processData() {
        std::cout << "Processing data using C-style loop:" << std::endl;
        for (int i = 0; i < size; ++i) {
            raw_data_array[i] *= PROCESSING_MULTIPLIER; // Example processing
            std::cout << "Processed item " << i << ": " << raw_data_array[i] << std::endl;
        }
    }

    void displayData() const {
        std::cout << "Current data: ";
        for (int i = 0; i < size; ++i) {
            std::cout << raw_data_array[i] << " ";
        }
        std::cout << std::endl;
    }
};

// Class demonstrating common C++ library usage
class ReportGenerator {
public:
    std::string report_title;
    std::vector<std::string> entries;
    std::map<int, std::string> status_codes;

    ReportGenerator(const std::string& title) : report_title(title) {
        status_codes[200] = "OK";
        status_codes[404] = "Not Found";
        status_codes[500] = "Internal Server Error";
        std::cout << "ReportGenerator created for: " << report_title << std::endl;
    }

    void addEntry(const std::string& entry) {
        entries.push_back(entry);
    }

    // Method to be refactored to include comments
    std::string generateReportContent() const;
};

// Function declaration lacking Doxygen comments (target for AI generation)
void printReport(const ReportGenerator& report);

#endif // DATA_STRUCTURES_HPP 