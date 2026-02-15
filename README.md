# 🔄 Page Replacement Simulator

<div align="center">

![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-success?style=for-the-badge)

**A comprehensive command-line simulator for analyzing and comparing page replacement algorithms in operating systems**

[Features](#-features) • [Usage](#-usage) • [Algorithms](#-algorithms) • [Examples](#-examples)

![Demo](https://img.shields.io/badge/Interactive-CLI-blue?style=flat-square)
![Algorithms](https://img.shields.io/badge/Algorithms-3-orange?style=flat-square)
![Visual](https://img.shields.io/badge/Colored_Output-Yes-green?style=flat-square)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Algorithms Implemented](#-algorithms-implemented)
- [Usage](#-usage)
- [Understanding the Output](#-understanding-the-output)
- [Examples](#-examples)
- [Customization](#-customization)
- [Troubleshooting](#-troubleshooting)
- [License](#-license)

---

## 🌟 Overview

The **Page Replacement Simulator** is an educational tool designed to help students and professionals understand how operating systems manage memory through page replacement algorithms. This interactive simulator implements three fundamental page replacement strategies used in virtual memory management and provides detailed visual feedback with performance metrics.

### What is Page Replacement?

In operating systems, when physical memory (RAM) is full and a new page needs to be loaded, the system must decide which existing page to remove. Different algorithms use different strategies to make this decision, affecting overall system performance.

---

## ✨ Features

### 🎯 Core Functionality

- **Three Classic Algorithms**
  - FIFO (First-In-First-Out)
  - LRU (Least Recently Used)
  - Optimal (Belady's Algorithm)

- **Comprehensive Analysis**
  - Real-time page table visualization
  - Page hit/fault tracking
  - Performance metrics calculation
  - Side-by-side algorithm comparison

### 🎨 User Experience

- **🌈 Color-Coded Output**
  - Green highlights for page hits
  - Red indicators for page faults
  - Cyan headers for better readability
  - Yellow markers for empty frames

- **📊 Detailed Statistics**
  - Total page faults count
  - Hit ratio (with percentage)
  - Fault rate (with percentage)
  - Comparative analysis mode

- **🛡️ Robust Input Validation**
  - Range checking for all inputs
  - Invalid input handling
  - User-friendly error messages
  - Continuous operation mode

### 🔧 Technical Features

- Dynamic memory allocation
- Efficient data structures
- Clean, modular code architecture
- Cross-platform ANSI color support
- Support for up to 100 pages and 1000 unique page numbers

---

## 🧮 Algorithms Implemented

### 1. FIFO (First-In-First-Out)

**Concept**: Replaces the oldest page in memory (the one that has been there the longest).

**How it works**:

- Maintains pages in a circular queue
- When a fault occurs, removes the page at the front of the queue
- Simple to implement but can suffer from Belady's Anomaly

**Use Case**: Simple systems where implementation complexity must be minimal

**Time Complexity**: O(1) for page replacement

---

### 2. LRU (Least Recently Used)

**Concept**: Replaces the page that hasn't been used for the longest period of time.

**How it works**:

- Tracks the last access time for each page using timestamps
- When a fault occurs, replaces the page with the smallest timestamp
- Generally performs better than FIFO

**Use Case**: Systems where recent access patterns are good predictors of future access

**Time Complexity**: O(n) for finding LRU page, where n is the number of frames

---

### 3. Optimal (Belady's Algorithm)

**Concept**: Replaces the page that will not be used for the longest time in the future.

**How it works**:

- Looks ahead in the reference string
- Replaces the page that will be accessed farthest in the future
- Theoretically optimal (minimum page faults) but requires future knowledge

**Use Case**: Benchmark for comparing other algorithms (not implementable in real systems)

**Time Complexity**: O(n\*m) where n is frames and m is remaining references

---

## 💻 Usage

### Main Menu

When you run the program, you'll see the main menu:

```
==============================
  Page Replacement Simulator
==============================
1. FIFO Algorithm
2. LRU Algorithm
3. Optimal Algorithm
4. Compare All
5. Exit
Enter your choice:
```

### Input Requirements

1. **Select Algorithm** (1-5)

2. **Number of Pages** (1-100)
   - Total number of page references to process

3. **Reference String** (space-separated integers 0-999)
   - Example: `7 0 1 2 0 3 0 4 2 3 0 3`

4. **Number of Frames** (1-100)
   - Available memory frames for pages

### Interactive Example

```bash
Enter your choice: 1
Enter number of pages (1-100): 12
Enter reference string (space-separated, pages 0-999):
7 0 1 2 0 3 0 4 2 3 0 3
Enter number of frames (1-100): 3
```

---

## 📊 Understanding the Output

### Table Format

The output displays a table showing the state of memory frames after each page reference:

```
+------+------+------+------+--------+
| Page |  F1  |  F2  |  F3  | Status |
+------+------+------+------+--------+
|    7 |   7  |   -  |   -  |  Fault |
|    0 |   7  |   0  |   -  |  Fault |
|    1 |   7  |   0  |   1  |  Fault |
|    2 |   2  |   0  |   1  |  Fault |
|    0 |   2  |   0  |   1  |  Hit   |
+------+------+------+------+--------+
```

**Column Descriptions**:

- **Page**: The current page being referenced
- **F1, F2, F3...**: Memory frame contents
- **Status**:
  - 🟢 **Hit**: Page already in memory (no fault)
  - 🔴 **Fault**: Page not in memory (replacement needed)
  - 🟡 **-**: Empty frame slot

### Performance Metrics

After the simulation, you'll see:

```
Total Page Faults = 9
Hit Ratio         = 0.25 (25%)
Fault Rate        = 0.75 (75%)
```

**Metrics Explained**:

- **Page Faults**: Number of times a page wasn't in memory
- **Hit Ratio**: Percentage of references found in memory (higher is better)
- **Fault Rate**: Percentage of references causing faults (lower is better)

---

## 🔍 Examples

### Example 1: Simple Reference String

**Input**:

```
Algorithm: FIFO
Pages: 12
Reference String: 7 0 1 2 0 3 0 4 2 3 0 3
Frames: 3
```

**Expected Output**:

- FIFO page faults: 9
- The algorithm will replace pages in the order they were loaded

### Example 2: Comparing Algorithms

**Input**:

```
Choice: 4 (Compare All)
Pages: 20
Reference String: 1 2 3 4 1 2 5 1 2 3 4 5 6 7 8 9 10 11 12 13
Frames: 4
```

**Expected Output**:

```
Comparison Summary
==============================
Algorithm       Page Faults
FIFO            15
LRU             14
Optimal         12
```

### Example 3: Belady's Anomaly Demonstration

**Input**:

```
Algorithm: FIFO
Reference String: 1 2 3 4 1 2 5 1 2 3 4 5

Test 1 - Frames: 3
Test 2 - Frames: 4
```

**Observation**: Sometimes FIFO can have MORE page faults with MORE frames (Belady's Anomaly)

---

## 🛠️ Customization

### Modifying Constants

Change maximum limits in the code:

```c
#define MAX 200           // Support up to 200 pages
#define MAX_PAGE 9999     // Support larger page numbers
```

### Disabling Colors

For terminals without ANSI support:

```c
// Comment out color definitions
#define RED ""
#define GREEN ""
// ... etc
```

## 🐛 Troubleshooting

### Issue: Colors Not Displaying

**Solution**:

- Use a modern terminal (Windows Terminal, iTerm2, GNOME Terminal)
- Enable ANSI color support in your terminal settings

### Issue: Compilation Errors

**Solution**:

```bash
# Ensure you have GCC installed
gcc --version

# Try with explicit standard
gcc -std=c99 page_replacement.c -o page_replacement
```

### Issue: Segmentation Fault

**Solution**:

- Check input values are within limits (1-100 for pages/frames, 0-999 for page numbers)
- Ensure sufficient system memory

### Issue: Unexpected Results

**Solution**:

- Verify your reference string is correct
- Try with known test cases
- Use "Compare All" mode to verify consistency

---

## 📄 License

This project is licensed under the MIT License - see below for details:

```
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```
