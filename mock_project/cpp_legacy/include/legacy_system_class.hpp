#ifndef LEGACY_SYSTEM_CLASS_HPP
#define LEGACY_SYSTEM_CLASS_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace MockProject {

// Forward declarations
class User;
class Item;

/**
 * @brief Modern C++ class that encapsulates the legacy system functionality
 * 
 * This class replaces the global state and functions from the original C-style
 * implementation with a proper object-oriented design.
 */
class LegacySystem {
public:
    /**
     * @brief Construct a new LegacySystem
     * 
     * Initializes the system in a non-initialized state.
     */
    LegacySystem();
    
    /**
     * @brief Destroy the LegacySystem
     * 
     * Cleans up all resources.
     */
    ~LegacySystem();
    
    /**
     * @brief Initialize the system
     * 
     * Prepares the system for use.
     */
    void initialize();
    
    /**
     * @brief Clean up system resources
     * 
     * Releases all allocated resources.
     */
    void cleanup();
    
    /**
     * @brief Add a new user to the system
     * 
     * @param id User ID
     * @param name User name
     * @param description User description
     * @param age User age
     * @return int The ID of the added user, or -1 if failed
     */
    int addUser(int id, const std::string& name, 
                const std::string& description, int age);
    
    /**
     * @brief Remove a user from the system
     * 
     * @param id ID of the user to remove
     * @return true if user was removed successfully
     * @return false if user was not found
     */
    bool removeUser(int id);
    
    /**
     * @brief Display user information to the console
     * 
     * @param id ID of the user to display
     */
    void displayUserInfo(int id) const;
    
    /**
     * @brief Find an item by ID
     * 
     * @param itemId ID of the item to find
     * @return Item* Pointer to the found item, or nullptr if not found
     */
    Item* findItem(int itemId);
    
    /**
     * @brief Get a user by ID
     * 
     * @param id ID of the user to find
     * @return User* Pointer to the found user, or nullptr if not found
     */
    User* getUserById(int id);
    
    /**
     * @brief Set an error message
     * 
     * @param message The error message to set
     */
    void setError(const std::string& message);
    
    /**
     * @brief Get the current error message
     * 
     * @return std::string The current error message
     */
    std::string getError() const;
    
    /**
     * @brief Process data using a callback function
     * 
     * @param data Data to process
     * @param processFn Callback function to process the data
     */
    void processData(const std::string& data, 
                    int (*processFn)(const std::string&, int));
    
    /**
     * @brief Save system data to a file
     * 
     * @param filename Path of the file to save to
     * @return true if save was successful
     * @return false if save failed
     */
    bool saveToFile(const std::string& filename) const;
    
    /**
     * @brief Load system data from a file
     * 
     * @param filename Path of the file to load from
     * @return true if load was successful
     * @return false if load failed
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * @brief Sort items by price
     */
    void sortItems();
    
    /**
     * @brief Update item details
     * 
     * @param itemId ID of the item to update
     * @param newName New name for the item
     * @param newPrice New price for the item
     * @param newQuantity New quantity for the item
     * @param notifyOwner Whether to notify the item's owner
     */
    void updateItemDetails(int itemId, const std::string& newName, 
                          float newPrice, int newQuantity, bool notifyOwner);

private:
    static constexpr int MAX_USERS = 100;
    static constexpr int MAX_ITEMS = 200;
    
    std::string errorMessage_;
    int debugMode_;
    int itemCount_;
    bool isInitialized_;
    std::vector<std::unique_ptr<User>> users_;
    std::vector<Item> items_;
};

/**
 * @brief User class that replaces the C-style struct
 */
class User {
public:
    /**
     * @brief Construct a new User
     * 
     * @param id User ID
     * @param name User name
     * @param description User description
     * @param age User age
     * @param isActive User active status
     */
    User(int id = 0, const std::string& name = "", 
         const std::string& description = "", int age = 0, 
         bool isActive = true);
    
    int id;
    std::string name;
    std::string description;
    int age;
    bool isActive;
};

/**
 * @brief Item class that replaces the C-style struct
 */
class Item {
public:
    /**
     * @brief Construct a new Item
     * 
     * @param id Item ID
     * @param name Item name
     * @param price Item price
     * @param quantity Item quantity
     * @param owner_id Owner ID
     */
    Item(int id = -1, const std::string& name = "", 
         float price = 0.0f, int quantity = 0, 
         int owner_id = 0);
    
    int id;
    std::string name;
    float price;
    int quantity;
    int owner_id;
};

} // namespace MockProject

#endif // LEGACY_SYSTEM_CLASS_HPP 