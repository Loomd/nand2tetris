# Hack Assembler

This repository contains an **Assembler** for the **Hack machine**, designed as part of the Nand2Tetris project. The Hack machine is a simple 16-bit computer with a minimal instruction set, perfect for educational purposes. This assembler translates Hack assembly language programs (`.asm` files) into binary machine language (`.hack` files) that can be executed by the Hack CPU.

[](...menustart)

## Table of Contents

- [Overview](#overview)
- [Building and Running](#building-and-running)
- [Hack Machine Language](#hack-machine-language)
  - [A-Instruction](#a-instruction)
  - [C-Instruction](#c-instruction)
- [Symbols in Hack Assembly](#symbols-in-hack-assembly)
  - [Predefined Symbols](#predefined-symbols)
  - [Label Symbols](#label-symbols)
  - [Variable Symbols](#variable-symbols)
- [Assembler Implementation](#assembler-implementation)
  - [Translating A-Instructions](#translating-a-instructions)
  - [Translating C-Instructions](#translating-c-instructions)
  - [Handling Symbols](#handling-symbols)
- [Input/Output](#inputoutput)

[](...menuend)

## Overview

The Hack assembler converts Hack assembly language into Hack binary machine language. Hack assembly is composed of two main types of instructions: **A-Instruction** and **C-Instruction**. The Hack machine operates with two distinct memories: **instruction memory** (read-only) and **data memory** (read/write), both of which are 16-bit wide.

### Hack Machine Overview:

- **16-bit von Neumann architecture**.
- **32K Instruction Memory (ROM)**.
- **32K Data Memory (RAM)**, with memory-mapped I/O devices for the screen and keyboard.
![n2t_hack_compute_archi](https://github.com/user-attachments/assets/5f56f6df-a028-4cff-8e35-1b9bd33d77e8)


---

## Building and Running

### Prerequisites

- **C++ compiler** (g++ recommended)
- Make (optional)

### Building from Source
- **Compile the source files**:
  - `g++ -I./include -c src/Assembler.cpp src/Parser.cpp src/BinCodes.cpp src/SymbolTables.cpp`
- **Link the object files**:
  - `g++ -o Assembler Assembler.o Parser.o BinCodes.o SymbolTables.o`

### Running the Assembler
- **Run the executable**:
  - `./Assembler.exe`
- **When prompted, enter the path to your '.asm' file**:
  - Enter the filename with extension ('filename.asm'): `path/to/your/program.asm`
- **If successful, a '.hack' file will be created in the same directory as your '.asm' file.**

### Example Usage
- `./Assembler.exe`
- Enter the filename with extension ('filename.asm'): `../tests/Add.asm`
- `Assembly completed successfully.`

---

## Hack Machine Language

### A-Instruction

The **A-Instruction** is used to load a constant into the A register or address a memory location.

- **Syntax**: `@value`
- **Binary Representation**: `0vvvvvvvvvvvvvvv`
  - The first bit is `0`, followed by a 15-bit value.

#### Usage:

1. **Set a Constant**: `@value` sets the A register to the specified value.
   - Example: `@5` → `0000000000000101`
2. **Address a Memory Location**: `@value` followed by a C-instruction using `M` accesses RAM.
   - Example: `@17` followed by `D=M` reads from `RAM[17]`.

### C-Instruction

The **C-Instruction** performs computations and manages conditional jumps.

- **Syntax**: `dest = comp ; jump`
  - `dest`: Destination register(s) (A, D, M).
  - `comp`: Computation (e.g., `D+1`, `A-1`, `D|M`).
  - `jump`: Jump condition (e.g., `JGT`, `JEQ`, `JMP`).

#### Binary Representation:

- **Format**: `111acccccccdddjjj`
  - The first three bits (`111`) identify the C-instruction.
  - `a`: Selects A-register (`0`) or M-register (`1`).
  - `comp`: Specifies the computation to perform (e.g., `D+1` → `001110`).
  - `dest`: Where to store the result (e.g., `D` → `010`).
  - `jump`: Jump condition (e.g., `JEQ` → `010`).

---

## Symbols in Hack Assembly

Hack assembly supports symbolic addressing for memory locations, labels, and variables.

### Predefined Symbols

Certain RAM addresses are predefined for easy reference:

- **Registers**: `R0` to `R15` map to RAM addresses 0 to 15.
- 
- ![n2t_virtual_register](https://github.com/user-attachments/assets/8ba70bcd-123e-424e-87f9-154d4a6dda82)
- 
- **Special Pointers**:
  - `SP` (Stack Pointer) = 0
  - `LCL` (Local Segment Pointer) = 1
  - `ARG` (Argument Segment Pointer) = 2
  - `THIS` = 3
  - `THAT` = 4
- **I/O Mapped Addresses**:
  - `SCREEN` = 16384 (Base address of the screen in memory).
  - `KBD` = 24576 (Keyboard input).

### Label Symbols

Labels are used to mark destinations in the code for `goto` commands and jumps. Labels are declared in parentheses.

- **Syntax**: `(LABEL)`
  - Example: `(LOOP)` creates a label `LOOP`.
  - `@LOOP` references this label and jumps to the instruction at `LOOP`.
  
  - ![n2t_symbolic_reference](https://github.com/user-attachments/assets/3a0df1c1-1921-47d5-9bc5-cbbfc7d8685c)


### Variable Symbols

- Symbols that aren't predefined or labels are treated as variables. Variables are stored in RAM starting from address 16. The assembler automatically assigns RAM addresses to these variables during the translation process.

- Example: use temp to exchange the content of R0 and R1

![n2t_variable](https://github.com/user-attachments/assets/1fa5e025-63c1-4a62-bfc6-f470d50d7a1b)

- here , temp don't have a corresponding label called temp. how it works ?
- Basically , we present the following pledge to the computer:
  - Please goto the memory unit, find some available memory register , say register n .
  - so from now on, each occurance of @temp in the program will be translated into @n
- Rules:
  - A reference to a symbol that has no corresponding lable declaration is treated as a reference to a variable.
  - Variable are allocated to the RAM from adderss 16 onward.
    - in this example , we have only 1 variable, so it ends up being allocated to RAM 16.
      
![n2t_variable_rom](https://github.com/user-attachments/assets/aa4f18cc-5695-4ae1-a886-651791fa03ee)


---

## Assembler Implementation

### Translating A-Instructions

1. If the instruction is in the form `@number`, convert the number into a 15-bit binary value.
2. If the instruction is in the form `@symbol`, resolve whether the symbol is a label, variable, or predefined symbol:
   - **Predefined symbols** are replaced with their corresponding addresses.
   - **Labels** are replaced with their ROM addresses.
   - **Variables** are assigned to the next available RAM address starting from 16.

### Translating C-Instructions

1. Parse the C-Instruction into its `dest`, `comp`, and `jump` components.
2. Convert each component into its corresponding binary code.
3. Assemble the final binary representation in the format `111acccccccdddjjj`.
   
![n2t_c_instruction_binary_syntax](https://github.com/user-attachments/assets/038736e3-ea1c-44ee-bc64-2beb3e6cc932)

![n2t_c_binary_comp](https://github.com/user-attachments/assets/f25271c8-547b-4acf-af16-b27b556ba5ef)

![n2t_c_binary_dest](https://github.com/user-attachments/assets/956c7aa5-58b4-4e57-a5e8-da3d289c4c58)

![n2t_c_binary_jump](https://github.com/user-attachments/assets/c7fd19fa-7ebd-40b7-88ea-a394e700f611)


### Handling Symbols

- **First Pass**: The assembler scans the program to identify all label declarations and assigns them to the correct instruction address in the ROM.
- **Second Pass**: During this pass, the assembler translates all instructions and allocates addresses for any variables encountered.

---

## Input/Output

- **Input**: Hack assembly code files (`.asm`).
- **Output**: Binary machine code files (`.hack`), with each line representing a 16-bit machine instruction.

![n2t_hack_character_set](https://github.com/user-attachments/assets/967c6aba-7075-496d-8b9a-5876e8651333)

---
