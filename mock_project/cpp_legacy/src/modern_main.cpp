#include <iostream>
#include <string>
#include <memory>
#include "../include/legacy_system_class.hpp"

// Example callback function for processData
int processCallback(const std::string& data, int length) {
    std::cout << "Processing data of length " << length << ": " << data << std::endl;
    return 0; // Success
}

int main() {
    std::cout << "--- Modern C++ Legacy System Demo ---" << std::endl;
    
    // Create and initialize the system
    MockProject::LegacySystem system;
    system.initialize();
    
    // Add users
    system.addUser(1, "John Doe", "First test user", 35);
    system.addUser(2, "Jane Smith", "Second test user with management access", 28);
    
    // Add items directly to the system
    system.updateItemDetails(1, "Laptop", 999.99f, 10, false);
    system.updateItemDetails(2, "Smartphone", 499.99f, 20, false);
    
    // Display user information
    system.displayUserInfo(1);
    
    // Process some data
    system.processData("Test data for processing", processCallback);
    
    // Safe string handling
    std::string tempDescription = "Temporary description";
    std::cout << "Temp: " << tempDescription << std::endl;
    
    // Modern type casting
    float f = 3.14f;
    int i = static_cast<int>(f);
    std::cout << "Converted float to int: " << i << std::endl;
    
    // Sort and save
    system.sortItems();
    system.saveToFile("modern_data.bin");
    
    // Clean up
    system.cleanup();
    
    std::cout << "--- Modern C++ Legacy System Demo End ---" << std::endl;
    return 0;
} 