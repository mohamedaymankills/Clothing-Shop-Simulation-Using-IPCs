#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOP_FIFO "shop_fifo"
#define BUFFER_SIZE 256

int main() {
    int shop_fd = open(SHOP_FIFO, O_WRONLY);
    if (shop_fd < 0) {
        perror("Error opening shop FIFO");
        exit(1);
    }

    char order[BUFFER_SIZE];
    printf("Enter item name and quantity (e.g., Shirt 2): ");
    fgets(order, BUFFER_SIZE, stdin);
    order[strcspn(order, "\n")] = 0;  // Remove newline

    printf("Sending order to FIFO...\n");
    if (write(shop_fd, order, strlen(order)) < 0) {
        perror("Error writing to FIFO");
    } else {
        printf("Order sent successfully.\n");
    }

    close(shop_fd);
    return 0;
}

