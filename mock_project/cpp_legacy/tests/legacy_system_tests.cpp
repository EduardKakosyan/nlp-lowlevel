#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "../include/legacy_system_class.hpp"

// Mock callback function for processData tests
int testProcessCallback(const std::string& data, int length) {
    return 0; // Success
}

int testProcessCallbackFail(const std::string& data, int length) {
    return -1; // Failure
}

// Test fixture for LegacySystem tests
class LegacySystemTest : public ::testing::Test {
protected:
    MockProject::LegacySystem system;
    
    void SetUp() override {
        system.initialize();
    }
    
    void TearDown() override {
        system.cleanup();
        
        // Clean up any test files
        std::remove("test_data.bin");
    }
    
    // Helper to add a test user
    void addTestUser() {
        system.addUser(1, "Test User", "Test Description", 30);
    }
    
    // Helper to add a test item
    void addTestItem() {
        system.updateItemDetails(1, "Test Item", 100.0f, 5, false);
    }
};

// Test User class
TEST(UserTest, Construction) {
    MockProject::User user(1, "Test", "Description", 25, true);
    
    EXPECT_EQ(1, user.id);
    EXPECT_EQ("Test", user.name);
    EXPECT_EQ("Description", user.description);
    EXPECT_EQ(25, user.age);
    EXPECT_TRUE(user.isActive);
}

// Test Item class
TEST(ItemTest, Construction) {
    MockProject::Item item(1, "TestItem", 99.99f, 10, 5);
    
    EXPECT_EQ(1, item.id);
    EXPECT_EQ("TestItem", item.name);
    EXPECT_FLOAT_EQ(99.99f, item.price);
    EXPECT_EQ(10, item.quantity);
    EXPECT_EQ(5, item.owner_id);
}

// Test system initialization and cleanup
TEST_F(LegacySystemTest, InitializeAndCleanup) {
    // The system is already initialized in SetUp
    addTestUser();
    addTestItem();
    
    // Cleanup should remove all users and items
    system.cleanup();
    
    // Verify cleanup by trying to find items/users
    EXPECT_EQ(nullptr, system.findItem(1));
    EXPECT_EQ(nullptr, system.getUserById(1));
}

// Test user management
TEST_F(LegacySystemTest, UserManagement) {
    // Add a user
    EXPECT_EQ(1, system.addUser(1, "Test User", "Description", 30));
    
    // Try to add duplicate user
    EXPECT_EQ(-1, system.addUser(1, "Another User", "Desc", 25));
    
    // Find the user by ID
    auto user = system.getUserById(1);
    ASSERT_NE(nullptr, user);
    EXPECT_EQ("Test User", user->name);
    
    // Remove the user
    EXPECT_TRUE(system.removeUser(1));
    
    // Verify removal
    EXPECT_EQ(nullptr, system.getUserById(1));
    
    // Try to remove non-existent user
    EXPECT_FALSE(system.removeUser(999));
}

// Test item management
TEST_F(LegacySystemTest, ItemManagement) {
    // Add an item via updateItemDetails
    system.updateItemDetails(1, "Test Item", 99.99f, 5, false);
    
    // Find the item
    auto item = system.findItem(1);
    ASSERT_NE(nullptr, item);
    EXPECT_EQ("Test Item", item->name);
    EXPECT_FLOAT_EQ(99.99f, item->price);
    
    // Update the item
    system.updateItemDetails(1, "Updated Item", 149.99f, 10, false);
    
    // Verify update
    item = system.findItem(1);
    ASSERT_NE(nullptr, item);
    EXPECT_EQ("Updated Item", item->name);
    EXPECT_FLOAT_EQ(149.99f, item->price);
    EXPECT_EQ(10, item->quantity);
    
    // Try to update non-existent item
    system.updateItemDetails(999, "Non-existent", 10.0f, 1, false);
    EXPECT_EQ(nullptr, system.findItem(999));
}

// Test error handling
TEST_F(LegacySystemTest, ErrorHandling) {
    // Generate an error by trying to remove a non-existent user
    system.removeUser(999);
    EXPECT_EQ("User not found", system.getError());
    
    // Generate an error by trying to update a non-existent item
    system.updateItemDetails(999, "Non-existent", 10.0f, 1, false);
    EXPECT_EQ("Item not found", system.getError());
}

// Test data processing
TEST_F(LegacySystemTest, DataProcessing) {
    // Test successful processing
    testing::internal::CaptureStdout();
    system.processData("Test data", testProcessCallback);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.empty()); // No error message expected
    
    // Test failed processing
    testing::internal::CaptureStdout();
    system.processData("Test data", testProcessCallbackFail);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(std::string::npos, output.find("processing failed")); // Error message expected
}

// Test file I/O
TEST_F(LegacySystemTest, FileIO) {
    // Add a user and an item
    system.addUser(1, "Test User", "Description", 30);
    system.updateItemDetails(1, "Test Item", 99.99f, 5, false);
    
    // Save to a file
    EXPECT_TRUE(system.saveToFile("test_data.bin"));
    
    // Create a new system and load from the file
    MockProject::LegacySystem newSystem;
    newSystem.initialize();
    EXPECT_TRUE(newSystem.loadFromFile("test_data.bin"));
    
    // Verify the loaded data
    auto user = newSystem.getUserById(1);
    ASSERT_NE(nullptr, user);
    EXPECT_EQ("Test User", user->name);
    
    auto item = newSystem.findItem(1);
    ASSERT_NE(nullptr, item);
    EXPECT_EQ("Test Item", item->name);
    EXPECT_FLOAT_EQ(99.99f, item->price);
    
    // Test loading from non-existent file
    EXPECT_FALSE(newSystem.loadFromFile("nonexistent_file.bin"));
    EXPECT_EQ("Failed to open file for reading", newSystem.getError());
}

// Test notifications for item updates
TEST_F(LegacySystemTest, ItemUpdateNotifications) {
    // Add a user and an item
    system.addUser(1, "Test User", "Description", 30);
    system.updateItemDetails(1, "Test Item", 99.99f, 5, false);
    
    // Set the item's owner
    auto item = system.findItem(1);
    ASSERT_NE(nullptr, item);
    item->owner_id = 1;
    
    // Update the item with owner notification
    testing::internal::CaptureStdout();
    system.updateItemDetails(1, "Updated Item", 149.99f, 10, true);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(std::string::npos, output.find("Notification sent to Test User"));
}

// Test sorting items by price
TEST_F(LegacySystemTest, SortItems) {
    // Add multiple items with different prices
    system.updateItemDetails(1, "Expensive Item", 199.99f, 5, false);
    system.updateItemDetails(2, "Cheap Item", 49.99f, 10, false);
    system.updateItemDetails(3, "Medium Item", 99.99f, 7, false);
    
    // Sort the items
    system.sortItems();
    
    // Verify sorting order by checking the first item
    auto item = system.findItem(1);
    ASSERT_NE(nullptr, item);
    EXPECT_EQ("Expensive Item", item->name);
}

// Main function to run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 