#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define INV_FIFO "inventory_fifo"
#define BUFFER_SIZE 256

int main() {
    int inv_fd = open(INV_FIFO, O_WRONLY);
    if (inv_fd < 0) {
        perror("Error opening inventory FIFO");
        exit(1);
    }

    char update[BUFFER_SIZE];
    printf("Enter item name, quantity to add, and new price (e.g., Shirt 5 19.99): ");
    fgets(update, BUFFER_SIZE, stdin);
    update[strcspn(update, "\n")] = 0;  // Remove newline

    printf("Sending inventory update to FIFO...\n");
    if (write(inv_fd, update, strlen(update)) < 0) {
        perror("Error writing to FIFO");
    } else {
        printf("Inventory update sent successfully.\n");
    }

    close(inv_fd);
    return 0;
}

