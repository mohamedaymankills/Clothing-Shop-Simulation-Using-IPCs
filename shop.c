#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256   // Define buffer size for reading FIFOs
#define SHOP_FIFO "shop_fifo"   // FIFO for customer orders
#define INV_FIFO "inventory_fifo" // FIFO for inventory updates

// Structure to store inventory items
typedef struct {
    char name[50];   // Item name
    int quantity;    // Quantity available in stock
    float price;     // Price per unit
} Item;

// Global inventory array to hold items
Item inventory[10];
int item_count = 0; // Number of items in inventory

// Function to load inventory from a file
void load_inventory() {
    FILE *file = fopen("inventory.txt", "r"); // Open inventory file in read mode
    if (!file) {
        perror("Error opening inventory file"); // Print error if file can't be opened
        exit(1); // Exit program
    }

    item_count = 0; // Reset item count
    // Read each line from the file (item name, quantity, price)
    while (fscanf(file, "%s %d %f", inventory[item_count].name, &inventory[item_count].quantity, &inventory[item_count].price) != EOF) {
        item_count++; // Increment item count
    }
    fclose(file); // Close the file
}

// Function to display the current inventory
void print_inventory() {
    printf("\n--- Shop Inventory ---\n");
    for (int i = 0; i < item_count; i++) {
        printf("%s - %d units - $%.2f\n", inventory[i].name, inventory[i].quantity, inventory[i].price);
    }
}

// Function to process customer orders
void process_order(char *order) {
    char item_name[50];
    int quantity;
    sscanf(order, "%s %d", item_name, &quantity); // Extract item name and quantity from order string

    // Search for the item in inventory
    for (int i = 0; i < item_count; i++) {
        if (strcmp(inventory[i].name, item_name) == 0) {
            if (inventory[i].quantity >= quantity) { // Check if enough stock is available
                inventory[i].quantity -= quantity; // Deduct ordered quantity
                printf("Order processed: %d x %s\n", quantity, item_name);
            } else {
                printf("Not enough stock for %s\n", item_name);
            }
            return; // Exit function once order is processed
        }
    }
    printf("Item not found: %s\n", item_name); // If item does not exist in inventory
}

// Function to process inventory updates from the inventory manager
void process_inventory_update(char *update) {
    char item_name[50];
    int quantity;
    float price;
    sscanf(update, "%s %d %f", item_name, &quantity, &price); // Extract update data

    // Search for the item in inventory
    for (int i = 0; i < item_count; i++) {
        if (strcmp(inventory[i].name, item_name) == 0) {
            inventory[i].quantity += quantity; // Add new stock quantity
            inventory[i].price = price; // Update item price
            printf("Updated %s: +%d units, new price: $%.2f\n", item_name, quantity, price);
            return;
        }
    }
    printf("Item not found: %s\n", item_name); // If item does not exist in inventory
}

int main() {
    // Create named FIFOs for communication
    mkfifo(SHOP_FIFO, 0666);
    mkfifo(INV_FIFO, 0666);

    load_inventory(); // Load inventory from file at startup

    // Open FIFOs for reading (non-blocking mode)
    int shop_fd = open(SHOP_FIFO, O_RDONLY | O_NONBLOCK);
    int inv_fd = open(INV_FIFO, O_RDONLY | O_NONBLOCK);
    
    if (shop_fd < 0 || inv_fd < 0) {
        perror("Error opening FIFOs"); // Error handling if FIFO cannot be opened
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        print_inventory(); // Display inventory periodically

        // Read from customer order FIFO
        int bytes_read = read(shop_fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the received string
            printf("Received order: %s\n", buffer);
            process_order(buffer); // Process customer order
        }

        // Read from inventory update FIFO
        bytes_read = read(inv_fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the received string
            printf("Received inventory update: %s\n", buffer);
            process_inventory_update(buffer); // Process inventory update
        }

        sleep(5); // Wait for a short duration before checking again
    }

    // Cleanup: Close FIFO file descriptors
    close(shop_fd);
    close(inv_fd);
    unlink(SHOP_FIFO);
    unlink(INV_FIFO);

    return 0;
}
