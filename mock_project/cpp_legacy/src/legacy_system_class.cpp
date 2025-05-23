#include "../include/legacy_system_class.hpp"
#include <algorithm>
#include <fstream>
#include <ctime>

namespace MockProject {

// User implementation
User::User(int id, const std::string& name, const std::string& description, 
           int age, bool isActive)
    : id(id), name(name), description(description), age(age), isActive(isActive) {
}

// Item implementation
Item::Item(int id, const std::string& name, float price, int quantity, int owner_id)
    : id(id), name(name), price(price), quantity(quantity), owner_id(owner_id) {
}

// LegacySystem implementation
LegacySystem::LegacySystem()
    : errorMessage_(""), debugMode_(0), itemCount_(0), isInitialized_(false) {
    // Initialize vectors with appropriate capacity
    users_.reserve(MAX_USERS);
    items_.reserve(MAX_ITEMS);
}

LegacySystem::~LegacySystem() {
    cleanup();
}

void LegacySystem::initialize() {
    if (isInitialized_) {
        return;
    }
    
    users_.clear();
    items_.clear();
    itemCount_ = 0;
    
    std::srand(std::time(nullptr));
    isInitialized_ = true;
    std::cout << "System initialized" << std::endl;
}

void LegacySystem::cleanup() {
    if (!isInitialized_) {
        return;
    }
    
    users_.clear();
    items_.clear();
    itemCount_ = 0;
    
    isInitialized_ = false;
    std::cout << "System cleaned up" << std::endl;
}

int LegacySystem::addUser(int id, const std::string& name, 
                          const std::string& description, int age) {
    if (users_.size() >= MAX_USERS) {
        setError("Maximum number of users reached");
        return -1;
    }
    
    auto it = std::find_if(users_.begin(), users_.end(),
                         [id](const std::unique_ptr<User>& user) { 
                             return user && user->id == id; 
                         });
    
    if (it != users_.end()) {
        setError("User ID already exists");
        return -1;
    }
    
    auto newUser = std::make_unique<User>(id, name, description, age, true);
    users_.push_back(std::move(newUser));
    
    return id;
}

bool LegacySystem::removeUser(int id) {
    auto it = std::find_if(users_.begin(), users_.end(),
                         [id](const std::unique_ptr<User>& user) { 
                             return user && user->id == id; 
                         });
    
    if (it != users_.end()) {
        users_.erase(it);
        return true;
    }
    
    setError("User not found");
    return false;
}

void LegacySystem::displayUserInfo(int id) const {
    auto it = std::find_if(users_.begin(), users_.end(),
                         [id](const std::unique_ptr<User>& user) { 
                             return user && user->id == id; 
                         });
    
    if (it == users_.end()) {
        std::cout << "User not found" << std::endl;
        return;
    }
    
    const User& user = *it->get();
    std::cout << "User ID: " << user.id << std::endl;
    std::cout << "Name: " << user.name << std::endl;
    std::cout << "Description: " << user.description << std::endl;
    std::cout << "Age: " << user.age << std::endl;
    std::cout << "Status: " << (user.isActive ? "Active" : "Inactive") << std::endl;
}

Item* LegacySystem::findItem(int itemId) {
    auto it = std::find_if(items_.begin(), items_.end(),
                         [itemId](const Item& item) { return item.id == itemId; });
    
    return (it != items_.end()) ? &(*it) : nullptr;
}

User* LegacySystem::getUserById(int id) {
    auto it = std::find_if(users_.begin(), users_.end(),
                         [id](const std::unique_ptr<User>& user) { 
                             return user && user->id == id; 
                         });
    
    return (it != users_.end()) ? it->get() : nullptr;
}

void LegacySystem::setError(const std::string& message) {
    errorMessage_ = message;
    if (debugMode_) {
        std::cerr << "ERROR: " << errorMessage_ << std::endl;
    }
}

std::string LegacySystem::getError() const {
    return errorMessage_;
}

void LegacySystem::processData(const std::string& data, 
                              int (*processFn)(const std::string&, int)) {
    std::string buffer = data;
    
    int result = processFn(buffer, buffer.length());
    
    if (result != 0) {
        std::cout << "Data processing failed with code: " << result << std::endl;
    }
}

bool LegacySystem::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        const_cast<LegacySystem*>(this)->setError("Failed to open file for writing");
        return false;
    }
    
    // Write users
    int userCount = static_cast<int>(users_.size());
    file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
    
    for (const auto& user : users_) {
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
    
    // Write items
    file.write(reinterpret_cast<const char*>(&itemCount_), sizeof(int));
    
    for (int i = 0; i < itemCount_; i++) {
        const Item& item = items_[i];
        
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

bool LegacySystem::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        setError("Failed to open file for reading");
        return false;
    }
    
    // Clean up existing data
    cleanup();
    initialize();
    
    // Read users
    int userCount;
    file.read(reinterpret_cast<char*>(&userCount), sizeof(int));
    
    for (int i = 0; i < userCount; i++) {
        int id;
        std::string name, description;
        int age;
        bool isActive;
        
        file.read(reinterpret_cast<char*>(&id), sizeof(int));
        
        // Read name
        int nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
        std::vector<char> nameBuf(nameLength + 1);
        file.read(nameBuf.data(), nameLength);
        nameBuf[nameLength] = '\0';
        name = nameBuf.data();
        
        // Read description
        int descriptionLength;
        file.read(reinterpret_cast<char*>(&descriptionLength), sizeof(int));
        std::vector<char> descBuf(descriptionLength + 1);
        file.read(descBuf.data(), descriptionLength);
        descBuf[descriptionLength] = '\0';
        description = descBuf.data();
        
        file.read(reinterpret_cast<char*>(&age), sizeof(int));
        file.read(reinterpret_cast<char*>(&isActive), sizeof(bool));
        
        auto user = std::make_unique<User>(id, name, description, age, isActive);
        users_.push_back(std::move(user));
    }
    
    // Read items
    file.read(reinterpret_cast<char*>(&itemCount_), sizeof(int));
    items_.resize(itemCount_);
    
    for (int i = 0; i < itemCount_; i++) {
        Item& item = items_[i];
        
        file.read(reinterpret_cast<char*>(&item.id), sizeof(int));
        
        // Read name
        int nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
        std::vector<char> nameBuf(nameLength + 1);
        file.read(nameBuf.data(), nameLength);
        nameBuf[nameLength] = '\0';
        item.name = nameBuf.data();
        
        file.read(reinterpret_cast<char*>(&item.price), sizeof(float));
        file.read(reinterpret_cast<char*>(&item.quantity), sizeof(int));
        file.read(reinterpret_cast<char*>(&item.owner_id), sizeof(int));
    }
    
    return true;
}

void LegacySystem::sortItems() {
    std::sort(items_.begin(), items_.begin() + itemCount_,
             [](const Item& a, const Item& b) {
                 return a.price < b.price;
             });
}

void LegacySystem::updateItemDetails(int itemId, const std::string& newName, 
                                    float newPrice, int newQuantity, bool notifyOwner) {
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

} // namespace MockProject 