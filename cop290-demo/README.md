# Spreadsheet Application

A C-based spreadsheet application that provides a command-line interface for creating, editing, and manipulating spreadsheets with support for formulas, dependencies, and various functions.

## Table of Contents

- [Spreadsheet Application](#spreadsheet-application)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Features](#features)
  - [Project Structure](#project-structure)
  - [Building the Application](#building-the-application)
  - [Running the Application](#running-the-application)
  - [Testing](#testing)
  - [Documentation](#documentation)
  - [Commands and Usage](#commands-and-usage)
    - [Basic Cell Operations](#basic-cell-operations)
    - [Functions](#functions)
    - [Navigation](#navigation)
    - [Control Commands](#control-commands)
  - [Cleaning Up](#cleaning-up)
  - [Complete Build](#complete-build)

## Overview

This spreadsheet application is a command-line tool that allows users to create and manipulate spreadsheets with cells containing values, formulas, and functions. It supports cell dependencies, error propagation, and various mathematical operations.

## Features

- Integer-based cell values
- Formula support with arithmetic operations (+, -, *, /)
- Cell references and dependency tracking
- Built-in functions (SUM, AVG, MIN, MAX, STDEV)
- Error detection and propagation
- Cycle detection in cell dependencies
- Navigation commands for viewing different parts of the spreadsheet
- Memory management for efficient operation

## Project Structure

The project is organized into the following directories:

```
cop290-demo/
├── clab/               # Source code for the spreadsheet application
│   ├── init.c/h        # Core definitions and initialization
│   ├── io.c/h          # Input/output handling
│   ├── process.c/h     # Command processing
│   ├── display.c/h     # Display functions
│   ├── dependent.c/h   # Dependency management
│   ├── stack.c/h       # Stack implementation for dependency resolution
│   └── Makefile        # Build instructions for source code
├── tests/              # Test suite for the application
│   ├── test_runner.c   # Main test runner
│   ├── test_io.c       # Tests for I/O functions
│   ├── test_process.c  # Tests for command processing
│   ├── test_display.c  # Tests for display functions
│   ├── test_dependent.c # Tests for dependency management
│   ├── test_integration.c # Integration tests
│   ├── test_*.txt      # Test input files
│   └── Makefile        # Build instructions for tests
├── report/             # Documentation and reports
│   ├── report.tex      # LaTeX source for the report
│   ├── report.pdf      # Generated PDF report
│   ├── assets/         # Images and other assets for the report
│   └── Makefile        # Build instructions for the report
├── target/             # Build output directory
│   └── release/        # Release build of the application
├── Makefile            # Main Makefile for the project
└── README.md           # This file
```

## Building the Application

The project uses a hierarchical Makefile system for building the application, running tests, and generating documentation.

To build the application:

```bash
make
```

This will compile the source code and place the executable at `target/release/spreadsheet`.

## Running the Application

To run the application:

```bash
make run
```

Or directly:

```bash
./target/release/spreadsheet [rows] [columns]
```

Where `[rows]` and `[columns]` are optional parameters to specify the visible dimensions of the spreadsheet (defaults to 10x10).

## Testing

The project includes a comprehensive test suite to verify the functionality of the application.

To run all tests:

```bash
make test
```

This will execute all test cases and generate a test report in `tests/test_output.txt`.

## Documentation

The project includes detailed documentation in the form of a PDF report.

To generate the report:

```bash
make report
```

This will compile the LaTeX source in the `report/` directory and generate `report.pdf`.


## Commands and Usage

### Basic Cell Operations

- `A1=42` - Set cell A1 to the value 42
- `B1=A1+10` - Set cell B1 to the value of A1 plus 10
- `C1=A1*B1` - Set cell C1 to the product of A1 and B1

### Functions

- `D1=SUM(A1:C1)` - Sum the values in the range A1 to C1
- `E1=AVG(A1:D1)` - Calculate the average of values in the range A1 to D1
- `F1=MIN(A1:E1)` - Find the minimum value in the range A1 to E1
- `G1=MAX(A1:F1)` - Find the maximum value in the range A1 to F1
- `H1=STDEV(A1:G1)` - Calculate the standard deviation of values in the range A1 to G1

### Navigation

- `w` - Scroll up
- `a` - Scroll left
- `s` - Scroll down
- `d` - Scroll right
- `scroll_to A10` - Scroll to cell A10

### Control Commands

- `q` - Quit the application
- `disable_output` - Disable screen output (for testing)
- `enable_output` - Enable screen output

## Cleaning Up

To clean build artifacts:

```bash
make clean           # Clean all build artifacts
make clean-clab      # Clean source code build artifacts
make clean-tests     # Clean test artifacts
make clean-report    # Clean report artifacts (keeping PDF)
make clean-target    # Clean target directory
make clean-all       # Clean everything including PDFs
```

## Complete Build

To build everything (application, run tests, generate report):

```bash
make complete
```

For more information about available commands, run:

```bash
make help            # Display help for main Makefile
make help-all        # Display help for all Makefiles
``` 