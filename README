# MIPS Processor Simulator

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Commands](#commands)
- [Instruction Set](#instruction-set)
- [Compilation and Execution](#compilation-and-execution)

---

## Overview

This project is a **MIPS (Microprocessor without Interlocked Pipeline Stages) processor simulator** implemented in C. It is designed to simulate the execution of MIPS assembly instructions, replicating the behavior of a basic MIPS CPU. The simulator supports various MIPS instruction types (R-type, I-type, and J-type) and includes core components like the ALU, register file, memory, and control unit.

The simulator fetches, decodes, executes, and updates the program counter and control signals in accordance with how a real MIPS processor operates.

---

## Features

- Simulates the MIPS instruction cycle (fetch, decode, execute).
- Supports R-type, I-type, and J-type MIPS instructions.
- Implements basic ALU operations including arithmetic, logical, and comparison instructions.
- Manages a simulated register file with 32 general-purpose registers.
- Supports memory read/write operations.
- Control signals for each instruction are generated using the instruction decode process.
- Outputs register contents and memory values in hexadecimal or decimal formats.
- Can step through instructions or run the program continuously.

---

## Project Structure

- **spimcore.c**: Contains the main functions to simulate the datapath, including the ALU, instruction fetch, decode, execution, memory access, and control signal logic.
- **spimcore.h**: Header file containing function declarations and macros for the MIPS simulator.
- **project.c**: Implements additional key functionality such as the ALU, memory read/write, and register file operations. It also manages the control signals and executes the MIPS instruction cycle, including instruction fetch, decode, execution, and updating the program counter.
- **input_file.asc**: Example assembly file with MIPS machine code to be loaded into memory.

---

## Usage

The simulator executes a MIPS program stored in memory by reading machine code from an input file. The user can issue commands via the terminal to control the execution, inspect register values, or display memory contents.

## Command List

The command prompt `cmd:` should appear. The simulation works with the following commands (both lowercase and uppercase letters are accepted):

| Command | Description                                                      |
|---------|------------------------------------------------------------------|
| r       | "Register" - Display register contents                           |
| m       | "Memory" - Display memory contents                               |
| s       | "Step" – Attempt to run the instruction located at the current PC|
| c       | "Continue" – Attempt to run all instructions, beginning with PC  |
| h       | "Halt" – Check to see if the simulation has halted               |
| p       | "Print" – Prints a copy of the input file                        |
| g       | "Controls" – Display the most recent control signals             |
| x       | "Quit" – Terminate the simulation                                |

---

## Instruction Set

The simulator supports the following MIPS instructions:

- **R-Type Instructions**:
  - `ADD`, `SUB`, `AND`, `OR`, `SLT`, `SLTU`
- **I-Type Instructions**:
  - `ADDI`, `LW`, `SW`, `BEQ`, `SLTI`, `SLTIU`, `LUI`
- **J-Type Instructions**:
  - `J`

Control signals are generated according to the instruction opcode, allowing the appropriate operations to be performed in each stage of the instruction cycle.

---

## Compilation and Execution

### Compile the Project:
```bash
gcc -o spimcore spimcore.c project.c
```

### Run the Project:
```bash
./spimcore input_file.asc
```