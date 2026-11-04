# Kalpavriksha Assignments

This repository contains my assignment work for the Kalpavriksha Program (Batch 2027).

The program focuses on building programming fundamentals using C language, with further learning in Data Structures and Operating Systems.

## Author

**Isha Bhaskar**

GitHub: https://github.com/ishaa021  
HackerRank: https://www.hackerrank.com/profile/ahsi_21075

## About the Program

The Kalpavriksha program is focused on strengthening programming fundamentals and understanding how basic programming concepts are used to solve problems.

The topics covered during the program include:

- C programming fundamentals
- Variables and data types
- Input and output
- Operators
- Conditional statements
- Loops
- Functions
- Strings
- Arrays
- Structures
- File handling
- Data Structures
- Operating Systems concepts

## Assignments

### Assignment 1 – Calculator

A console-based calculator written in C that takes an expression as input and performs:

- Addition, subtraction, multiplication and division
- Integer operations
- Operator precedence
- Whitespace handling
- Basic error handling such as division by zero

### Assignment 2 – CRUD Operations Using File Handling

A simple user management program using C structures and file handling.

Each user has a unique ID, name and age. The program supports:

- Create
- Read
- Update
- Delete

User records are stored in a text file named `users.txt`.

### Assignment 7 – Smart Memory Manager

A menu-driven memory management simulation implemented in C using dynamic memory allocation.

The program manages a dynamically allocated memory pool and maintains a separate metadata array to track memory blocks.

Features:

- Dynamic memory pool creation using malloc().
- Dynamic metadata management using malloc() and realloc().
- Memory allocation using First Fit, Best Fit and Worst Fit strategies.
- Splitting free blocks during allocation.
- Releasing allocated memory using unique allocation IDs.
- Merging adjacent free blocks to reduce fragmentation.
- Resizing existing allocations using shrink, expand and move operations.
- Preserving existing data during resizing using memmove().
- Detecting external fragmentation.
- Displaying memory maps and memory statistics.
- Tracking allocated memory, free memory and the largest available free block.

## How to Compile and Run

```bash
gcc filename.c -o program
./program
