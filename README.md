# 🔄 Page Replacement Simulator

<div align="center">

![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-success?style=for-the-badge)

**A command-line simulator for exploring page replacement algorithms in operating systems**

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

---

## 🌟 Overview

The **Page Replacement Simulator** is an educational tool designed to help students and professionals understand how operating systems manage memory through page replacement algorithms. This interactive simulator implements three fundamental page replacement strategies used in virtual memory management and provides visual feedback with performance metrics.

### What is Page Replacement?

In operating systems, when physical memory (RAM) is full and a new page needs to be loaded, the system must decide which existing page to remove. Different algorithms use different strategies to make this decision, affecting overall system performance.

---

## ✨ Features

### 🎯 Core Functionality

- **Three Classic Algorithms**
  - FIFO (First-In-First-Out)
  - LRU (Least Recently Used)
  - Optimal (Belady's Algorithm)

- **Simulation Output**
  - Real-time frame visualization
  - Page hit/fault tracking
  - Performance metrics calculation

### 🎨 User Experience

- **🌈 Color-Coded Output**
  - Green highlights for page hits
  - Red indicators for page faults
  - Cyan headers for better readability
  - Yellow markers for empty frames
- **🖥️ Interactive TUI Simulation**
  - Step-by-step mode (press Enter to advance)
  - Auto-play mode with a fixed delay set at start
  - Live frame visualization, hit/fault status, and running stats

- **📊 Detailed Statistics**
  - Total page faults count
  - Hit ratio (with percentage)
  - Fault rate (with percentage)

- **🛡️ Robust Input Validation**
  - Range checking for all inputs
  - Invalid input handling
  - User-friendly error messages

### 🔧 Technical Features

- Dynamic memory allocation
- Efficient data structures
- Clean, modular code architecture
- Cross-platform ANSI color support
- Support for up to 100 references and page numbers in `0..999`

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
4. Exit
Enter your choice:
```

### Output Modes

After selecting an algorithm and entering input, choose how to visualize:

```
1. TUI step-by-step
2. TUI auto (animated)
```

Auto mode lets you set a delay in milliseconds (50–2000ms).
Controls:

- Auto mode: `Ctrl+C` quit
- Step mode: `Enter` next step, `Q` quit

### Automated Reference Strings

The simulator now generates the reference string automatically based on your inputs:

- Number of references
- Maximum page number
- Number of frames

This makes the simulation fully automated and ready for visualization.

### Input Requirements

1. **Select Algorithm** (1-4)

2. **Number of References** (1-100)
   - Total number of page references to process

3. **Max Page Number** (1-999)
   - Reference string is auto-generated within `0..max`

4. **Number of Frames** (1-100)
   - Available memory frames for pages

### Interactive Example

```bash
Enter your choice: 1
Enter number of references (1-100): 12
Enter max page number (1-999): 9
Enter number of frames (1-100): 3
```

---

## 📊 Understanding the Output

The simulation renders a full-screen TUI dashboard showing:

- Current step, page, and status (hit/fault)
- Live frame contents
- Running stats (faults, hits, hit ratio)
- The generated reference string with the current page highlighted

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
References: 12
Max Page: 9
Frames: 3
```

**Notes**:

- The reference string is generated automatically within `0..Max Page`.
- The algorithm will replace pages in the order they were loaded.

### Example 2: Running Another Algorithm

**Input**:

```
Algorithm: LRU
References: 20
Max Page: 13
Frames: 4
```

**Notes**:

- The reference string is generated automatically.
- Use the TUI auto mode for full visualization.

### Example 3: Belady's Anomaly Demonstration

**Input**:

```
Algorithm: FIFO
References: 12
Max Page: 5

Test 1 - Frames: 3
Test 2 - Frames: 4
```

**Observation**: Sometimes FIFO can have MORE page faults with MORE frames (Belady's Anomaly)

---

## 🛠️ Customization

### Modifying Constants

Change maximum limits in the code:

```c
#define MAX 100           // Support up to 100 references
#define MAX_PAGE 999      // Support page numbers 0..999
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

- Check input values are within limits (1-100 for references/frames, 0-999 for page numbers)
- Ensure sufficient system memory

### Issue: Unexpected Results

**Solution**:

- Remember the reference string is randomly generated
- Try with known test cases
