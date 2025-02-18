#README for PA1
#README for PA1
# Project Overview

This project consists of several C files each designed to handle specific aspects of a larger computation task using different methods such as single-thread execution, multithreading, and multitasking. Below is a description of each file and its corresponding part in the assignment.

## File Descriptions

### `baseline.c`

- Performs the computation in a single-threaded manner without any optimization.
- This file serves as the baseline for performance comparison against optimized versions.

### `multithreading.c`

- Implements the computation using multithreading to utilize multiple CPU cores simultaneously.
- Demonstrates the use of POSIX threads to split the computation task among available processors.

### `multitasking1.c`

- Manages the task by dividing the work between multiple processes using the system's fork mechanism.
- Focuses on process-based multitasking, dividing the computation across child processes.

### `multitasking2.c`

- Similar to `multitasking1`, but uses file-based inter-process communication to manage results from multiple chunks.
- Advances multitasking by incorporating file operations for result collection.

### `calculateChunk.c`

- Computes a specific chunk of the total task, designed to be called multiple times concurrently by different tasks/processes.
- Works in conjunction with `multitasking1.c` and `multitasking2.c` for distributed computing.

### `functions.c` and `functions.h`

- Provides utility functions such as `bigAdd`, `split`, and `init` used across various parts of the project to handle large number additions and number splitting.
- Acts as a shared library providing common functionality needed by multiple other scripts.

### `BuildAll.sh`

- Bash script to compile all the C programs with appropriate flags.
- Automation script to build all parts of the assignment efficiently.

## Compilation Instructions

Run the provided Bash script `BuildAll.sh` to compile all the programs as follows:

```bash
./BuildAll.sh

