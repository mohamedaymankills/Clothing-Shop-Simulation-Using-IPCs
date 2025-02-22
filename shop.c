#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256
#define SHOP_FIFO "shop_fifo"
#define INV_FIFO "inventory_fifo"

// Structure to store inventory items
typedef struct {
    char name[50];
    int quantity;
    float price;
} Item;

// Global inventory
Item inventory[10];
int item_count = 0;

// Function to load inventory from a file
void load_inventory() {
    FILE *file = fopen("inventory.txt", "r");
    if (!file) {
        perror("Error opening inventory file");
        exit(1);
    }

    item_count = 0;
    while (fscanf(file, "%s %d %f", inventory[item_count].name, &inventory[item_count].quantity, &inventory[item_count].price) != EOF) {
        item_count++;
    }
    fclose(file);
}

// Function to display inventory
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
    sscanf(order, "%s %d", item_name, &quantity);

    for (int i = 0; i < item_count; i++) {
        if (strcmp(inventory[i].name, item_name) == 0) {
            if (inventory[i].quantity >= quantity) {
                inventory[i].quantity -= quantity;
                printf("Order processed: %d x %s\n", quantity, item_name);
            } else {
                printf("Not enough stock for %s\n", item_name);
            }
            return;
        }
    }
    printf("Item not found: %s\n", item_name);
}

// Function to process inventory updates
void process_inventory_update(char *update) {
    char item_name[50];
    int quantity;
    float price;
    sscanf(update, "%s %d %f", item_name, &quantity, &price);

    for (int i = 0; i < item_count; i++) {
        if (strcmp(inventory[i].name, item_name) == 0) {
            inventory[i].quantity += quantity;
            inventory[i].price = price;
            printf("Updated %s: +%d units, new price: $%.2f\n", item_name, quantity, price);
            return;
        }
    }
    printf("Item not found: %s\n", item_name);
}

int main() {
    mkfifo(SHOP_FIFO, 0666);
    mkfifo(INV_FIFO, 0666);

    load_inventory();

    int shop_fd = open(SHOP_FIFO, O_RDONLY | O_NONBLOCK);
    int inv_fd = open(INV_FIFO, O_RDONLY | O_NONBLOCK);
    
    if (shop_fd < 0 || inv_fd < 0) {
        perror("Error opening FIFOs");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        print_inventory();

        // Read from customer FIFO
        int bytes_read = read(shop_fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Received order: %s\n", buffer);
            process_order(buffer);
        }

        // Read from inventory FIFO
        bytes_read = read(inv_fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Received inventory update: %s\n", buffer);
            process_inventory_update(buffer);
        }

        sleep(5);
    }

    close(shop_fd);
    close(inv_fd);
    unlink(SHOP_FIFO);
    unlink(INV_FIFO);

    return 0;
}

