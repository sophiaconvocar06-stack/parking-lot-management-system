****CC104–Parking-Lot-Management-System

Final Project for CC104: Data Structure and Algorithms
Overview
**
This is a console-based application developed in C for the CC104 Final Project. It simulates a backend management system for a parking facility, allowing users to register and authenticate accounts, park vehicles, manage waiting queues when the parking is full, process vehicle exits with automated billing, and maintain a history of completed transactions.

Data Structures & Algorithms Used

This project implements the following core concepts:
Data Structures:

Stack (Array-based):
Used as the main parking structure to store vehicles currently parked in the lot. It follows the Last-In, First-Out (LIFO) principle, meaning the most recently parked vehicle is the first to be removed.

Queue (Array-based):
Used for the waiting line when the parking lot reaches maximum capacity. It follows the First-In, First-Out (FIFO) principle, ensuring fairness in vehicle entry.

Array (History Storage):
Used to store completed parking transactions such as plate number, vehicle type, parking duration, and total payment.

Algorithms:

Linear Search:
Used to locate vehicles in both the parking stack and waiting queue based on plate number.

Bubble Sort:
Implemented to sort transaction history based on total payment or number of hours for better data organization and analysis.

Additional Features
User Authentication (File Handling):
Allows users to register and log in using credentials stored in a text file.

Automated Billing System:
Calculates parking fees based on duration and vehicle type (Car or Motorcycle).

Receipt Generation:
Displays a formatted receipt in the console and saves it into a file (receipt.txt) for record keeping.

Graph Visualization:
Provides a simple ASCII-based visualization of total earnings.

How to Compile and Run
To run this application, you need a standard C compiler (like GCC) installed on your system.

Steps:

Save the file as:
parkinglotmanagementsystem.c
Open your terminal or command prompt.

Navigate to the folder where the file is located:
cd "c:\Users\Admin\Desktop\"

Compile the program:
gcc parkinglotmanagementsystem.c -o parkinglotmanagementsystem

Run the program:
parking_app or if does not work use this command instead  parkinglotmanagementsystem
