#include <iostream>
#include <vector>
#include "../include/data_structures.hpp"
// Forward declare functions from utils.cpp if not in a header
// For a real project, utils.cpp functions would be in utils.hpp and included here.
void processItems(int* items, int count);
struct MyResource; // Forward declaration for the struct defined in utils.cpp
MyResource* createResource(int id, const std::string& data);
void releaseResource(MyResource* res);
double calculateMetrics(const std::vector<double>& data_points);
int_fast16_t getLegacyStatusCode();

int main() {
    std::cout << "--- Mock Project C++ Demo Start --- " << std::endl;

    // Demonstrate LegacyPoint
    LegacyPoint p = {10, 20};
    std::cout << "LegacyPoint: x = " << p.x << ", y = " << p.y << std::endl;

    // Demonstrate DataProcessor
    int initial_data[] = {1, 2, 3, 4, 5};
    DataProcessor dp(initial_data, 5);
    dp.displayData();
    dp.processData();
    dp.displayData();

    // Demonstrate ReportGenerator
    ReportGenerator annual_report("Annual Financials 2023");
    annual_report.addEntry("Revenue exceeded targets by 15%.");
    annual_report.addEntry("New product line launched successfully.");
    annual_report.addEntry("Market share increased by 3%.");
    printReport(annual_report); // Uses the function to be documented

    // Demonstrate utils functions
    std::cout << "\n--- Demonstrating utils.cpp functions ---" << std::endl;
    int items_array[] = {10, 20, 30};
    processItems(items_array, 3);
    std::cout << "Items array after processItems: " << items_array[0] << ", " << items_array[1] << ", " << items_array[2] << std::endl;

    MyResource* res1 = createResource(1, "Resource Alpha");
    if (res1) res1->process();
    releaseResource(res1); // Manual resource management

    std::vector<double> metrics_data = {10.5, 12.3, 11.0, 13.2, 10.8};
    double result_metric = calculateMetrics(metrics_data);
    std::cout << "Final calculated metric: " << result_metric << std::endl;

    int_fast16_t statusCode = getLegacyStatusCode(); 
    std::cout << "Retrieved legacy status code: " << statusCode << std::endl;

    std::cout << "\n--- Mock Project C++ Demo End --- " << std::endl;
    return 0;
} 