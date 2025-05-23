#ifndef LEGACY_SYSTEM_H
#define LEGACY_SYSTEM_H

#include <cstddef>  // For size_t
#include <string>   // For std::string

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum limits for the system */
static constexpr int MAX_USERS = 100;
static constexpr int MAX_NAME_LENGTH = 50;
static constexpr int MAX_ITEMS = 200;
static constexpr int BUFFER_SIZE = 1024;

/* Type definitions */
using BOOL = bool;  // For transition, we'll keep BOOL as an alias to bool

/* User structure with std::string */
struct User {
    int id;
    std::string name;        // Changed from char[MAX_NAME_LENGTH]
    std::string description; // Changed from char*
    int age;
    bool isActive;
};

/* Item structure */
struct Item {
    int id;
    std::string name;        // Changed from char[MAX_NAME_LENGTH]
    float price;
    int quantity;
    int owner_id;
};

/* Global variable declarations */
extern std::string g_errorMessage;  // Changed from char[256]
extern int g_debugMode;
extern int g_itemCount;
extern bool g_isInitialized;
extern User* g_users[MAX_USERS];
extern Item g_items[MAX_ITEMS];
extern int g_userCount;

/* Function declarations */
void initSystem();
void cleanupSystem();
int addUser(int id, const std::string& name, const std::string& description, int age);  // Changed parameters
bool removeUser(int id);
void displayUserInfo(int id);
Item* findItem(int itemId);
User* getUserById(int id);
void setError(const std::string& message);  // Changed parameter
std::string getError();  // Changed return type
void processData(const std::string& data, int (*processFn)(const std::string&, int));  // Changed parameter
bool saveToFile(const std::string& filename);  // Changed parameter
bool loadFromFile(const std::string& filename);  // Changed parameter
void sortItems();
void updateItemDetails(int itemId, const std::string& newName, float newPrice, int newQuantity, bool notifyOwner);  // Changed parameter

#ifdef __cplusplus
}
#endif

#endif /* LEGACY_SYSTEM_H */ 