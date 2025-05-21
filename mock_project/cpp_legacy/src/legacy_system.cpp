#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_ITEMS 200
#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

typedef int BOOL;

// Global variables
char g_errorMessage[256];
int g_debugMode = 0;
int g_itemCount = 0;
BOOL g_isInitialized = FALSE;

// User structure with C-style strings
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char* description;
    int age;
    BOOL isActive;
} User;

// Item structure
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
    int owner_id;
} Item;

// Global arrays
User* g_users[MAX_USERS];
Item g_items[MAX_ITEMS];
int g_userCount = 0;

// Function prototypes
void initSystem();
void cleanupSystem();
int addUser(int id, char* name, char* description, int age);
BOOL removeUser(int id);
void displayUserInfo(int id);
Item* findItem(int itemId);
User* getUserById(int id);
void setError(char* message);
char* getError();
void processData(char* data, int (*processFn)(char*, int));
BOOL saveToFile(char* filename);
BOOL loadFromFile(char* filename);
void sortItems();

// Initialize the system
void initSystem() {
    if (g_isInitialized == TRUE) {
        return;
    }
    
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        g_users[i] = NULL;
    }
    
    for (i = 0; i < MAX_ITEMS; i++) {
        g_items[i].id = -1;
    }
    
    srand(time(NULL));
    g_isInitialized = TRUE;
    printf("System initialized\n");
}

// Clean up resources
void cleanupSystem() {
    if (g_isInitialized == FALSE) {
        return;
    }
    
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (g_users[i] != NULL) {
            if (g_users[i]->description != NULL) {
                free(g_users[i]->description);
            }
            free(g_users[i]);
            g_users[i] = NULL;
        }
    }
    
    g_isInitialized = FALSE;
    printf("System cleaned up\n");
}

// Add a new user to the system
int addUser(int id, char* name, char* description, int age) {
    if (g_userCount >= MAX_USERS) {
        setError("Maximum number of users reached");
        return -1;
    }
    
    int i;
    for (i = 0; i < g_userCount; i++) {
        if (g_users[i] != NULL && g_users[i]->id == id) {
            setError("User ID already exists");
            return -1;
        }
    }
    
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        setError("Memory allocation failed");
        return -1;
    }
    
    newUser->id = id;
    strncpy(newUser->name, name, MAX_NAME_LENGTH);
    newUser->name[MAX_NAME_LENGTH - 1] = '\0';
    
    newUser->description = (char*)malloc(strlen(description) + 1);
    if (newUser->description == NULL) {
        free(newUser);
        setError("Memory allocation failed");
        return -1;
    }
    
    strcpy(newUser->description, description);
    newUser->age = age;
    newUser->isActive = TRUE;
    
    g_users[g_userCount] = newUser;
    g_userCount++;
    
    return id;
}

// Remove a user from the system
BOOL removeUser(int id) {
    int i;
    for (i = 0; i < g_userCount; i++) {
        if (g_users[i] != NULL && g_users[i]->id == id) {
            if (g_users[i]->description != NULL) {
                free(g_users[i]->description);
            }
            free(g_users[i]);
            
            // Shift remaining users
            int j;
            for (j = i; j < g_userCount - 1; j++) {
                g_users[j] = g_users[j + 1];
            }
            
            g_users[g_userCount - 1] = NULL;
            g_userCount--;
            
            return TRUE;
        }
    }
    
    setError("User not found");
    return FALSE;
}

// Display user information
void displayUserInfo(int id) {
    User* user = getUserById(id);
    if (user == NULL) {
        printf("User not found\n");
        return;
    }
    
    printf("User ID: %d\n", user->id);
    printf("Name: %s\n", user->name);
    printf("Description: %s\n", user->description);
    printf("Age: %d\n", user->age);
    printf("Status: %s\n", user->isActive ? "Active" : "Inactive");
}

// Find an item by ID
Item* findItem(int itemId) {
    int i;
    for (i = 0; i < g_itemCount; i++) {
        if (g_items[i].id == itemId) {
            return &g_items[i];
        }
    }
    
    return NULL;
}

// Get user by ID
User* getUserById(int id) {
    int i;
    for (i = 0; i < g_userCount; i++) {
        if (g_users[i] != NULL && g_users[i]->id == id) {
            return g_users[i];
        }
    }
    
    return NULL;
}

// Set error message
void setError(char* message) {
    strcpy(g_errorMessage, message);
    if (g_debugMode) {
        printf("ERROR: %s\n", g_errorMessage);
    }
}

// Get error message
char* getError() {
    return g_errorMessage;
}

// Process data with callback function
void processData(char* data, int (*processFn)(char*, int)) {
    char buffer[BUFFER_SIZE];
    strcpy(buffer, data);
    
    int result = processFn(buffer, strlen(buffer));
    
    if (result != 0) {
        printf("Data processing failed with code: %d\n", result);
    }
}

// Save data to file
BOOL saveToFile(char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        setError("Failed to open file for writing");
        return FALSE;
    }
    
    fwrite(&g_userCount, sizeof(int), 1, file);
    
    int i;
    for (i = 0; i < g_userCount; i++) {
        User* user = g_users[i];
        fwrite(&user->id, sizeof(int), 1, file);
        fwrite(user->name, sizeof(char), MAX_NAME_LENGTH, file);
        
        int descriptionLength = strlen(user->description);
        fwrite(&descriptionLength, sizeof(int), 1, file);
        fwrite(user->description, sizeof(char), descriptionLength, file);
        
        fwrite(&user->age, sizeof(int), 1, file);
        fwrite(&user->isActive, sizeof(BOOL), 1, file);
    }
    
    fwrite(&g_itemCount, sizeof(int), 1, file);
    fwrite(g_items, sizeof(Item), g_itemCount, file);
    
    fclose(file);
    return TRUE;
}

// Load data from file
BOOL loadFromFile(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        setError("Failed to open file for reading");
        return FALSE;
    }
    
    // Clean up existing data
    cleanupSystem();
    initSystem();
    
    fread(&g_userCount, sizeof(int), 1, file);
    
    int i;
    for (i = 0; i < g_userCount; i++) {
        User* user = (User*)malloc(sizeof(User));
        if (user == NULL) {
            setError("Memory allocation failed during load");
            fclose(file);
            return FALSE;
        }
        
        fread(&user->id, sizeof(int), 1, file);
        fread(user->name, sizeof(char), MAX_NAME_LENGTH, file);
        
        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, file);
        
        user->description = (char*)malloc(descriptionLength + 1);
        if (user->description == NULL) {
            free(user);
            setError("Memory allocation failed during load");
            fclose(file);
            return FALSE;
        }
        
        fread(user->description, sizeof(char), descriptionLength, file);
        user->description[descriptionLength] = '\0';
        
        fread(&user->age, sizeof(int), 1, file);
        fread(&user->isActive, sizeof(BOOL), 1, file);
        
        g_users[i] = user;
    }
    
    fread(&g_itemCount, sizeof(int), 1, file);
    fread(g_items, sizeof(Item), g_itemCount, file);
    
    fclose(file);
    return TRUE;
}

// Bubble sort items by price
void sortItems() {
    int i, j;
    for (i = 0; i < g_itemCount - 1; i++) {
        for (j = 0; j < g_itemCount - i - 1; j++) {
            if (g_items[j].price > g_items[j + 1].price) {
                // Swap items
                Item temp = g_items[j];
                g_items[j] = g_items[j + 1];
                g_items[j + 1] = temp;
            }
        }
    }
}

// Example of a complex function with many parameters
void updateItemDetails(int itemId, char* newName, float newPrice, int newQuantity, BOOL notifyOwner) {
    Item* item = findItem(itemId);
    if (item == NULL) {
        setError("Item not found");
        return;
    }
    
    strncpy(item->name, newName, MAX_NAME_LENGTH);
    item->name[MAX_NAME_LENGTH - 1] = '\0';
    item->price = newPrice;
    item->quantity = newQuantity;
    
    if (notifyOwner) {
        User* owner = getUserById(item->owner_id);
        if (owner != NULL) {
            printf("Notification sent to %s about item update\n", owner->name);
        }
    }
}

// Main function demonstration
int main() {
    initSystem();
    
    addUser(1, "John Doe", "First test user", 35);
    addUser(2, "Jane Smith", "Second test user with management access", 28);
    
    g_items[g_itemCount].id = 1;
    strcpy(g_items[g_itemCount].name, "Laptop");
    g_items[g_itemCount].price = 999.99;
    g_items[g_itemCount].quantity = 10;
    g_items[g_itemCount].owner_id = 1;
    g_itemCount++;
    
    g_items[g_itemCount].id = 2;
    strcpy(g_items[g_itemCount].name, "Smartphone");
    g_items[g_itemCount].price = 499.99;
    g_items[g_itemCount].quantity = 20;
    g_items[g_itemCount].owner_id = 2;
    g_itemCount++;
    
    displayUserInfo(1);
    
    // Example memory leak (not freeing allocated memory)
    char* tempDescription = (char*)malloc(100);
    strcpy(tempDescription, "Temporary description");
    printf("Temp: %s\n", tempDescription);
    
    // Dangerous buffer operations
    char buffer[10];
    strcpy(buffer, "This string is too long for the buffer");
    
    // Type casting the old way
    float f = 3.14;
    int i = (int)f;
    
    // Using raw pointer arithmetic
    char* ptr = buffer;
    ptr += 5;
    printf("Char at offset 5: %c\n", *ptr);
    
    sortItems();
    saveToFile("data.bin");
    
    cleanupSystem();
    return 0;
} 