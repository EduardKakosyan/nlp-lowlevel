import * as fs from 'fs';
import * as path from 'path';

const MAX_USERS = 100;
const MAX_NAME_LENGTH = 50; // Will be used for validation, not strict char arrays
const MAX_ITEMS = 200;
// const BUFFER_SIZE = 1024; // Less relevant with dynamic strings

interface User {
    id: number;
    name: string;
    description: string;
    age: number;
    isActive: boolean;
}

interface Item {
    id: number;
    name: string;
    price: number;
    quantity: number;
    ownerId: number; // Changed from owner_id for camelCase convention
}

type ProcessFunction = (data: string, length: number) => number;

class LegacySystem {
    private errorMessage: string = "";
    private debugMode: boolean = false;
    private itemCount: number = 0;
    private isInitialized: boolean = false;

    private users: User[] = [];
    private items: Item[] = [];
    // userCount can be derived from users.length

    constructor(debugMode: boolean = false) {
        this.debugMode = debugMode;
    }

    public getUsers(): ReadonlyArray<User> {
        return this.users;
    }

    public getItems(): ReadonlyArray<Item> {
        return this.items;
    }

    public initSystem(): void {
        if (this.isInitialized) {
            return;
        }
        this.users = [];
        this.items = [];
        this.itemCount = 0;
        // srand(time(NULL)) equivalent for random seeding is not directly needed
        // unless specific predictable random numbers are required.
        this.isInitialized = true;
        console.log("System initialized");
    }

    public cleanupSystem(): void {
        if (!this.isInitialized) {
            return;
        }
        this.users = [];
        this.items = [];
        this.itemCount = 0;
        // No manual memory management (free) needed for users or descriptions
        this.isInitialized = false;
        console.log("System cleaned up");
    }

    private setError(message: string): void {
        this.errorMessage = message;
        if (this.debugMode) {
            console.error(`ERROR: ${this.errorMessage}`);
        }
    }

    public getError(): string {
        return this.errorMessage;
    }

    public addUser(id: number, name: string, description: string, age: number): number {
        if (this.users.length >= MAX_USERS) {
            this.setError("Maximum number of users reached");
            return -1;
        }

        if (this.users.some(user => user.id === id)) {
            this.setError("User ID already exists");
            return -1;
        }

        if (name.length >= MAX_NAME_LENGTH) {
            console.warn(`Warning: User name for ID ${id} exceeds MAX_NAME_LENGTH and will be truncated.`);
            name = name.substring(0, MAX_NAME_LENGTH -1);
        }

        const newUser: User = {
            id,
            name,
            description,
            age,
            isActive: true,
        };

        this.users.push(newUser);
        return id;
    }

    public removeUser(id: number): boolean {
        const userIndex = this.users.findIndex(user => user.id === id);
        if (userIndex === -1) {
            this.setError("User not found");
            return false;
        }

        this.users.splice(userIndex, 1);
        // No need to manually shift elements or nullify last element
        return true;
    }

    public displayUserInfo(id: number): void {
        const user = this.getUserById(id);
        if (!user) {
            console.log("User not found");
            return;
        }

        console.log(`User ID: ${user.id}`);
        console.log(`Name: ${user.name}`);
        console.log(`Description: ${user.description}`);
        console.log(`Age: ${user.age}`);
        console.log(`Status: ${user.isActive ? "Active" : "Inactive"}`);
    }

    public findItem(itemId: number): Item | null {
        const item = this.items.find(item => item.id === itemId);
        return item || null;
    }

    public getUserById(id: number): User | null {
        const user = this.users.find(user => user.id === id);
        return user || null;
    }
    
    public processData(data: string, processFn: ProcessFunction): void {
        // Buffer copy is not strictly necessary as strings are immutable
        // but to mimic the original's intent of working on a copy:
        let buffer = data; 
        const result = processFn(buffer, buffer.length);

        if (result !== 0) {
            console.log(`Data processing failed with code: ${result}`);
        } else {
            console.log(`Data processing succeeded.`);
        }
    }

    public saveToFile(filename: string): boolean {
        try {
            const dataToSave = {
                users: this.users,
                items: this.items,
                itemCount: this.itemCount // Or derive from items.length upon load
            };
            fs.writeFileSync(filename, JSON.stringify(dataToSave, null, 2), 'utf8');
            return true;
        } catch (error) {
            this.setError(`Failed to save file: ${error instanceof Error ? error.message : String(error)}`);
            return false;
        }
    }

    public loadFromFile(filename: string): boolean {
        try {
            if (!fs.existsSync(filename)) {
                this.setError("File not found for loading.");
                return false;
            }
            const fileContent = fs.readFileSync(filename, 'utf8');
            const dataLoaded = JSON.parse(fileContent);

            this.cleanupSystem(); // Clean existing data
            this.initSystem();   // Reinitialize

            this.users = dataLoaded.users || [];
            this.items = dataLoaded.items || [];
            this.itemCount = dataLoaded.items?.length || 0; // Recalculate item count

            return true;
        } catch (error) {
            this.setError(`Failed to load file: ${error instanceof Error ? error.message : String(error)}`);
            // Perform cleanup if loading fails mid-way to ensure consistent state
            this.cleanupSystem();
            this.initSystem();
            return false;
        }
    }

    public sortItems(): void {
        this.items.sort((a, b) => a.price - b.price);
    }
    
    public updateItemDetails(itemId: number, newName: string, newPrice: number, newQuantity: number, notifyOwner: boolean): void {
        const item = this.findItem(itemId);
        if (!item) {
            this.setError("Item not found");
            return;
        }

        if (newName.length >= MAX_NAME_LENGTH) {
            console.warn(`Warning: Item name for ID ${itemId} exceeds MAX_NAME_LENGTH and will be truncated.`);
            item.name = newName.substring(0, MAX_NAME_LENGTH -1);
        } else {
            item.name = newName;
        }
        item.price = newPrice;
        item.quantity = newQuantity;

        if (notifyOwner) {
            const owner = this.getUserById(item.ownerId);
            if (owner) {
                console.log(`Notification sent to ${owner.name} about item update for item ID ${item.id}`);
            } else {
                console.log(`Owner (ID: ${item.ownerId}) not found for item ID ${item.id}, notification not sent.`);
            }
        }
    }

    // Helper to add items directly for demonstration, similar to C++ main
    public addItem(id: number, name: string, price: number, quantity: number, ownerId: number): void {
        if (this.items.length >= MAX_ITEMS) {
            this.setError("Maximum number of items reached.");
            return;
        }
        if (this.items.some(item => item.id === id)) {
            this.setError(`Item with ID ${id} already exists.`);
            return;
        }
         if (name.length >= MAX_NAME_LENGTH) {
            console.warn(`Warning: Item name for ID ${id} exceeds MAX_NAME_LENGTH and will be truncated.`);
            name = name.substring(0, MAX_NAME_LENGTH -1);
        }
        this.items.push({ id, name, price, quantity, ownerId });
        this.itemCount = this.items.length;
    }
}

// Main function demonstration
function main(): void {
    const system = new LegacySystem(true); // Enable debug mode
    system.initSystem();

    const user1Id = system.addUser(1, "John Doe", "First test user", 35);
    if (user1Id === -1) console.error(system.getError());
    
    const user2Id = system.addUser(2, "Jane Smith", "Second test user with management access", 28);
    if (user2Id === -1) console.error(system.getError());

    system.addItem(1, "Laptop", 999.99, 10, 1);
    system.addItem(2, "Smartphone", 499.99, 20, 2);
    system.addItem(3, "Tablet", 299.00, 15, 1);


    if (user1Id !== -1) system.displayUserInfo(user1Id);
    if (user2Id !== -1) system.displayUserInfo(user2Id);
    
    console.log("\nItem List (Unsorted):");
    system.getItems().forEach(item => console.log(item));
    
    system.sortItems();
    console.log("\nItem List (Sorted by Price):");
    system.getItems().forEach(item => console.log(item));

    system.updateItemDetails(1, "Gaming Laptop", 1199.99, 8, true);
    const updatedItem = system.findItem(1);
    if(updatedItem) console.log("\nUpdated Item:", updatedItem);

    const dataFilePath = path.join(__dirname, 'data.json');
    console.log(`\nSaving data to ${dataFilePath}...`);
    if (system.saveToFile(dataFilePath)) {
        console.log("Data saved successfully.");
    } else {
        console.error("Failed to save data:", system.getError());
    }

    // Demonstrate loading
    console.log("\nCleaning system and loading data...");
    system.cleanupSystem(); // Clean current state
    system.initSystem();    // Re-init before load
    
    if (system.loadFromFile(dataFilePath)) {
        console.log("Data loaded successfully.");
        console.log("Users after load:", system.getUsers());
        console.log("Items after load:", system.getItems());
        const users = system.getUsers();
        if (users.length > 0) system.displayUserInfo(users[0].id);
    } else {
        console.error("Failed to load data:", system.getError());
    }

    // Example for processData
    const sampleData = "This is some sample data to process.";
    const myProcessFn: ProcessFunction = (d, l) => {
        console.log(`Processing data: "${d}" (length: ${l})`);
        // Example: return 0 for success, non-zero for failure
        return d.includes("error") ? 1 : 0; 
    };
    system.processData(sampleData, myProcessFn);
    system.processData("This data contains an error trigger.", myProcessFn);


    system.cleanupSystem();
    
    // Remove the data file after demo
    try {
        if(fs.existsSync(dataFilePath)) fs.unlinkSync(dataFilePath);
    } catch (e) {
        console.error("Could not delete test data file", e)
    }
}

// Run the main demonstration
main();

// To run this:
// 1. Save as legacy_system.ts in a directory (e.g., TypescriptCode)
// 2. Make sure you have Node.js and TypeScript installed.
// 3. Install types for node: `npm install --save-dev @types/node` (or `yarn add --dev @types/node`) in your project
// 4. Compile: `tsc TypescriptCode/legacy_system.ts`
// 5. Run: `node TypescriptCode/legacy_system.js` 