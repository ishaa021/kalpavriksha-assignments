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

### Assignment 9 – ICC ODI Player Performance Analyzer

A menu-driven C program to manage and analyze ODI cricket player performances across 10 international teams.

Features
- Add players to teams with Player ID validation.
- Display all players of a selected team.
- Display teams by average batting strike rate.
- Display Top K players by role and Performance Index.
- Display players of a specific role across all teams in descending order of Performance Index.

Concepts Used

- Structures and Pointers
- Singly Linked Lists
- Dynamic Memory Allocation
- Sorting and Searching
- Pairwise Merging of Sorted Linked Lists

Performance Index

- Batsman: (Batting Average × Strike Rate) / 100
- Bowler: (Wickets × 2) + (100 - Economy Rate)
- All-rounder: ((Batting Average × Strike Rate) / 100) + (Wickets × 2)

Dataset
Players_data.h contains 200 predefined players across 10 teams.

## How to Compile and Run

```bash
gcc filename.c -o program
./program
