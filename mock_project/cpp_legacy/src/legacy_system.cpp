#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> // Add for std::sort
#include "../include/legacy_system.h"  // Include the header file

// Global variables
std::string g_errorMessage = "";
int g_debugMode = 0;
int g_itemCount = 0;
bool g_isInitialized = false;

// Global arrays
User* g_users[MAX_USERS];
Item g_items[MAX_ITEMS];
int g_userCount = 0;

// Function prototypes
void initSystem();
void cleanupSystem();
int addUser(int id, const std::string& name, const std::string& description, int age);
bool removeUser(int id);
void displayUserInfo(int id);
Item* findItem(int itemId);
User* getUserById(int id);
void setError(const std::string& message);
// The error is that `std::string` is used, but the necessary header `<string>` (which defines `std::string`) 
// has likely not been included in this .cpp file. This causes the compiler to report an error,
// such as 'namespace "std" has no member "string"', because it cannot find the definition for `std::string`.
void processData(const std::string& data, int (*processFn)(const std::string&, int));
bool saveToFile(const std::string& filename);
bool loadFromFile(const std::string& filename);
void sortItems();

// Initialize the system
void initSystem() {
    if (g_isInitialized) {
        return;
    }
    
    for (int i = 0; i < MAX_USERS; i++) {
        g_users[i] = nullptr;
    }
    
    for (int i = 0; i < MAX_ITEMS; i++) {
        g_items[i].id = -1;
    }
    
    std::srand(std::time(nullptr));
    g_isInitialized = true;
    std::cout << "System initialized" << std::endl;
}

// Clean up resources
void cleanupSystem() {
    if (!g_isInitialized) {
        return;
    }
    
    for (int i = 0; i < MAX_USERS; i++) {
        if (g_users[i] != nullptr) {
            delete g_users[i];
            g_users[i] = nullptr;
        }
    }
    
    g_isInitialized = false;
    std::cout << "System cleaned up" << std::endl;
}

// Add a new user to the system
int addUser(int id, const std::string& name, const std::string& description, int age) {
    if (g_userCount >= MAX_USERS) {
        setError("Maximum number of users reached");
        return -1;
    }
    
    for (int i = 0; i < g_userCount; i++) {
        if (g_users[i] != nullptr && g_users[i]->id == id) {
            setError("User ID already exists");
            return -1;
        }
    }
    
    User* newUser = new User();
    if (newUser == nullptr) {
        setError("Memory allocation failed");
        return -1;
    }
    
    newUser->id = id;
    newUser->name = name;
    newUser->description = description;
    newUser->age = age;
    newUser->isActive = true;
    
    g_users[g_userCount] = newUser;
    g_userCount++;
    
    return id;
}

// Remove a user from the system
bool removeUser(int id) {
    for (int i = 0; i < g_userCount; i++) {
        if (g_users[i] != nullptr && g_users[i]->id == id) {
            delete g_users[i];
            
            // Shift remaining users
            for (int j = i; j < g_userCount - 1; j++) {
                g_users[j] = g_users[j + 1];
            }
            
            g_users[g_userCount - 1] = nullptr;
            g_userCount--;
            
            return true;
        }
    }
    
    setError("User not found");
    return false;
}

// Display user information
void displayUserInfo(int id) {
    User* user = getUserById(id);
    if (user == nullptr) {
        std::cout << "User not found" << std::endl;
        return;
    }
    
    std::cout << "User ID: " << user->id << std::endl;
    std::cout << "Name: " << user->name << std::endl;
    std::cout << "Description: " << user->description << std::endl;
    std::cout << "Age: " << user->age << std::endl;
    std::cout << "Status: " << (user->isActive ? "Active" : "Inactive") << std::endl;
}

// Find an item by ID using std::find_if
Item* findItem(int itemId) {
    auto it = std::find_if(g_items, g_items + g_itemCount,
                         [itemId](const Item& item) { return item.id == itemId; });
    
    return (it != g_items + g_itemCount) ? &(*it) : nullptr;
}

// Get user by ID using std::find_if
User* getUserById(int id) {
    auto it = std::find_if(g_users, g_users + g_userCount,
                         [id](const User* user) { return user != nullptr && user->id == id; });
    
    return (it != g_users + g_userCount) ? *it : nullptr;
}

// Set error message
void setError(const std::string& message) {
    g_errorMessage = message;
    if (g_debugMode) {
        std::cerr << "ERROR: " << g_errorMessage << std::endl;
    }
}

// Get error message
std::string getError() {
    return g_errorMessage;
}

// Process data with callback function
void processData(const std::string& data, int (*processFn)(const std::string&, int)) {
    std::string buffer = data;
    
    int result = processFn(buffer, buffer.length());
    
    if (result != 0) {
        std::cout << "Data processing failed with code: " << result << std::endl;
    }
}

// Save data to file
bool saveToFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        setError("Failed to open file for writing");
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(&g_userCount), sizeof(int));
    
    for (int i = 0; i < g_userCount; i++) {
        User* user = g_users[i];
        
        file.write(reinterpret_cast<const char*>(&user->id), sizeof(int));
        
        // Write name
        int nameLength = static_cast<int>(user->name.length());
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
        file.write(user->name.c_str(), nameLength);
        
        // Write description
        int descriptionLength = static_cast<int>(user->description.length());
        file.write(reinterpret_cast<const char*>(&descriptionLength), sizeof(int));
        file.write(user->description.c_str(), descriptionLength);
        
        file.write(reinterpret_cast<const char*>(&user->age), sizeof(int));
        file.write(reinterpret_cast<const char*>(&user->isActive), sizeof(bool));
    }
    
    file.write(reinterpret_cast<const char*>(&g_itemCount), sizeof(int));
    
    for (int i = 0; i < g_itemCount; i++) {
        Item& item = g_items[i];
        
        file.write(reinterpret_cast<const char*>(&item.id), sizeof(int));
        
        // Write name
        int nameLength = static_cast<int>(item.name.length());
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
        file.write(item.name.c_str(), nameLength);
        
        file.write(reinterpret_cast<const char*>(&item.price), sizeof(float));
        file.write(reinterpret_cast<const char*>(&item.quantity), sizeof(int));
        file.write(reinterpret_cast<const char*>(&item.owner_id), sizeof(int));
    }
    
    return true;
}

// Load data from file
bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        setError("Failed to open file for reading");
        return false;
    }
    
    // Clean up existing data
    cleanupSystem();
    initSystem();
    
    file.read(reinterpret_cast<char*>(&g_userCount), sizeof(int));
    
    for (int i = 0; i < g_userCount; i++) {
        User* user = new User();
        if (user == nullptr) {
            setError("Memory allocation failed during load");
            return false;
        }
        
        file.read(reinterpret_cast<char*>(&user->id), sizeof(int));
        
        // Read name
        int nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
        char* nameBuf = new char[nameLength + 1];
        file.read(nameBuf, nameLength);
        nameBuf[nameLength] = '\0';
        user->name = nameBuf;
        delete[] nameBuf;
        
        // Read description
        int descriptionLength;
        file.read(reinterpret_cast<char*>(&descriptionLength), sizeof(int));
        char* descBuf = new char[descriptionLength + 1];
        file.read(descBuf, descriptionLength);
        descBuf[descriptionLength] = '\0';
        user->description = descBuf;
        delete[] descBuf;
        
        file.read(reinterpret_cast<char*>(&user->age), sizeof(int));
        file.read(reinterpret_cast<char*>(&user->isActive), sizeof(bool));
        
        g_users[i] = user;
    }
    
    file.read(reinterpret_cast<char*>(&g_itemCount), sizeof(int));
    
    for (int i = 0; i < g_itemCount; i++) {
        file.read(reinterpret_cast<char*>(&g_items[i].id), sizeof(int));
        
        // Read name
        int nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
        char* nameBuf = new char[nameLength + 1];
        file.read(nameBuf, nameLength);
        nameBuf[nameLength] = '\0';
        g_items[i].name = nameBuf;
        delete[] nameBuf;
        
        file.read(reinterpret_cast<char*>(&g_items[i].price), sizeof(float));
        file.read(reinterpret_cast<char*>(&g_items[i].quantity), sizeof(int));
        file.read(reinterpret_cast<char*>(&g_items[i].owner_id), sizeof(int));
    }
    
    return true;
}

// Sort items by price using std::sort
void sortItems() {
    std::sort(g_items, g_items + g_itemCount, 
        [](const Item& a, const Item& b) {
            return a.price < b.price;
        });
}

// Example of a complex function with many parameters
void updateItemDetails(int itemId, const std::string& newName, float newPrice, int newQuantity, bool notifyOwner) {
    Item* item = findItem(itemId);
    if (item == nullptr) {
        setError("Item not found");
        return;
    }
    
    item->name = newName;
    item->price = newPrice;
    item->quantity = newQuantity;
    
    if (notifyOwner) {
        User* owner = getUserById(item->owner_id);
        if (owner != nullptr) {
            std::cout << "Notification sent to " << owner->name << " about item update" << std::endl;
        }
    }
}

// Main function demonstration
int main() {
    initSystem();
    
    addUser(1, "John Doe", "First test user", 35);
    addUser(2, "Jane Smith", "Second test user with management access", 28);
    
    // Initialize items using direct assignment
    g_items[g_itemCount].id = 1;
    g_items[g_itemCount].name = "Laptop";
    g_items[g_itemCount].price = 999.99;
    g_items[g_itemCount].quantity = 10;
    g_items[g_itemCount].owner_id = 1;
    g_itemCount++;
    
    g_items[g_itemCount].id = 2;
    g_items[g_itemCount].name = "Smartphone";
    g_items[g_itemCount].price = 499.99;
    g_items[g_itemCount].quantity = 20;
    g_items[g_itemCount].owner_id = 2;
    g_itemCount++;
    
    displayUserInfo(1);
    
    // Using std::string instead of C-style strings avoids memory leaks
    std::string tempDescription = "Temporary description";
    std::cout << "Temp: " << tempDescription << std::endl;
    
    // Safe buffer handling with std::string
    std::string safeBuffer = "This string can be of any length without buffer overflow risk";
    std::cout << "Safe buffer: " << safeBuffer << std::endl;
    
    // Modern type casting
    float f = 3.14;
    int i = static_cast<int>(f);
    std::cout << "Converted float to int: " << i << std::endl;
    
    sortItems();
    saveToFile("data.bin");
    
    cleanupSystem();
    return 0;
} 