#ifndef LEGACY_SYSTEM_H
#define LEGACY_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum limits for the system */
#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_ITEMS 200
#define BUFFER_SIZE 1024

/* Boolean constants */
#define TRUE 1
#define FALSE 0

/* Type definitions */
typedef int BOOL;

/* User structure with C-style strings */
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char* description;
    int age;
    BOOL isActive;
} User;

/* Item structure */
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
    int owner_id;
} Item;

/* Global variable declarations */
extern char g_errorMessage[256];
extern int g_debugMode;
extern int g_itemCount;
extern BOOL g_isInitialized;
extern User* g_users[MAX_USERS];
extern Item g_items[MAX_ITEMS];
extern int g_userCount;

/* Function declarations */
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
void updateItemDetails(int itemId, char* newName, float newPrice, int newQuantity, BOOL notifyOwner);

#ifdef __cplusplus
}
#endif

#endif /* LEGACY_SYSTEM_H */ 