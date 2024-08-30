# Simple Operating System Project

## Overview

This project is a simple operating system implementation, designed to provide a hands-on learning experience for understanding the fundamentals of operating system development. It includes components such as a bootloader, kernel, and basic system utilities.

## Project Structure

The project is organized into several key components:

1. Boot Sector (`source/boot`)
2. Loader (`source/loader`)
3. Kernel (`source/kernel`)
4. Common Utilities (`source/comm`)

## Key Features

- 16-bit real mode bootloader
- Transition to 32-bit protected mode
- Basic memory detection
- Simple kernel initialization

## Building the Project

This project uses CMake for build management. To build the project:

1. Create a build directory:
   ```
   mkdir build && cd build
   ```

2. Run CMake:
   ```
   cmake ..
   ```

3. Build the project:
   ```
   make
   ```

## Running the OS

The project includes scripts for running the OS using QEMU:

- For macOS: Use `script/qemu-debug-osx.sh`
- For Linux: Use `script/qemu-debug-linux.sh`
- For Windows: Use `script/qemu-debug-win.bat`

## Project Components

### Boot Sector
The boot sector is responsible for loading the loader into memory and transferring control to it. It's implemented in assembly and C.

### Loader
The loader sets up the environment for the kernel, including:
- Detecting system memory
- Entering protected mode
- Loading the kernel into memory

### Kernel
The kernel is the core of the operating system. In its current state, it performs basic initialization tasks.

### Common Utilities
This section includes shared definitions and utility functions used across different parts of the project.

## Development

This project is a work in progress and is intended for educational purposes. Contributions and improvements are welcome!


## Acknowledgments

This project is inspired by various OS development tutorials and resources available online.