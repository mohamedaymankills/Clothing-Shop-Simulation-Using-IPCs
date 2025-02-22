# Clothing-Shop-Simulation-Using-IPCs

Clothing Shop Simulation

📌 Project Overview

This project simulates a multi-process Clothing Shop using FIFO (named pipes) for Inter-Process Communication (IPC). It consists of three independent processes that communicate with each other to handle shop management, customer orders, and inventory updates in real time.

🛠 Components

The project consists of three main processes:

Shop Process 🏬

Manages the shop's inventory.

Reads the initial stock and prices from a file.

Displays inventory periodically.

Handles orders from customers.

Processes restocking and price updates from the inventory manager.

Customer Process 🛒

Allows a user to place an order.

Takes input for item name and quantity.

Sends the order request to the shop via FIFO.

Inventory Manager Process 📦

Allows a user (admin) to update stock and prices.

Takes input for item name, quantity to add, and new price.

Sends updates to the shop via FIFO.

📂 File Structure

📁 Clothing_Shop_Simulation/
│── shop.c                # Shop Process Implementation
│── customer.c            # Customer Process Implementation
│── inventory_manager.c   # Inventory Manager Implementation
│── inventory.txt         # Initial inventory file
│── Makefile              # Build system
│── README.md             # Project documentation

⚙️ How It Works (Step-by-Step)

1️⃣ Initialization

The Shop process starts first.

It reads the initial inventory from inventory.txt.

The shop begins displaying inventory periodically.

2️⃣ Customer Process (Placing Orders)

The customer enters the item name and quantity.

The order request is sent via FIFO to the Shop.

The shop processes the request and updates inventory.

3️⃣ Inventory Manager Process (Restocking & Price Updates)

Admin enters item name, quantity to add, and new price.

The update request is sent via FIFO to the Shop.

The shop processes the update and modifies inventory.

4️⃣ Shop Processing (Main Process)

Listens for customer orders and processes them.

Listens for inventory updates and applies changes.

Continually displays updated inventory.

🛠 Setup & Installation

1️⃣ Clone the Repository

git clone https://github.com/mohamedaymankills/Clothing_Shop_Simulation.git
cd Clothing_Shop_Simulation

2️⃣ Compile the Code

Use the provided Makefile to compile all programs:

make

This will generate three executable files:

shop

customer

inventory_manager

3️⃣ Run the Shop Process (First Terminal)

./shop

This will start the shop and display the inventory periodically.

4️⃣ Run the Customer Process (Second Terminal)

./customer

The program will prompt you to enter an order:

Enter item name and quantity (e.g., Shirt 2):

5️⃣ Run the Inventory Manager Process (Third Terminal)

./inventory_manager

The program will prompt you to enter an update:

Enter item name, quantity to add, and new price (e.g., Shirt 5 19.99):

📌 Example Run

Shop (Terminal 1)

--- Shop Inventory ---
Shirt - 10 units - $20.50
Jeans - 5 units - $45.99
Jacket - 3 units - $79.99

Customer (Terminal 2)

Enter item name and quantity (e.g., Shirt 2): Shirt 2

Shop Updates Inventory

Processing order: Shirt 2
Order completed! Remaining: Shirt - 8 units

Inventory Manager (Terminal 3)

Enter item name, quantity to add, and new price (e.g., Shirt 5 19.99): Shirt 5 19.99

Shop Updates Inventory

Restocking: Shirt +5 units, New Price: $19.99

🛠 Inter-Process Communication (IPC) using FIFO

The Customer Process sends orders to FIFO_ORDER.

The Inventory Manager sends updates to FIFO_UPDATE.

The Shop Process reads from both FIFOs and updates inventory accordingly.

🎯 Future Improvements

✅ Support multiple customers simultaneously.
✅ Add a graphical user interface (GUI).
✅ Implement more IPC methods (shared memory, message queues).

📜 License

This project is licensed under the MIT License.

📞 Contact

For any questions or contributions, reach out to:
📧 Email: mohamedaymankills@gmail.com🐙 GitHub: @mohamedaymankills

🚀 Happy Coding!

