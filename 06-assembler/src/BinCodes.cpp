/**
 * @file BinCodes.cpp
 * @brief Assembler module for translating Hack assembly instructions to machine code.
 * 
 * This module handles the conversion of both A-instructions and C-instructions to 
 * their 16-bit binary representation as required by the Hack assembly language.
 * 
 * Key Concepts:
 * - A-instructions (e.g., `@5`) specify a memory address or value to be loaded.
 * - C-instructions (e.g., `D=M+1`) specify computation, storage, and optional jump conditions.
 * 
 * Uses:
 * - `stoi`: Converts a string to an integer.
 * - `bitset<15>`: A template that converts an integer to a binary string. Here, `15` denotes 
 *   that the integer is stored in a 15-bit binary representation.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "../include/BinCodes.h"
#include "../include/SymbolTables.h"


using namespace std;

/**
 * @brief Translates an A-instruction into binary code.
 * 
 * Converts an A-instruction (e.g., `@5`) into a 16-bit binary string. A-instructions
 * start with a leading '0' bit, followed by a 15-bit binary representation of the address.
 * 
 * @param token The A-instruction in the format "@value".
 * @return string A 16-bit binary string representing the A-instruction.
 */
string translateAInstruction(const string& token){
    int address = stoi(token.substr(1));  // `stoi` converts the address part of token (after '@') to an integer
    return "0" + bitset<15>(address).to_string();  // Convert to 15-bit binary and prepend '0' for A-instruction
}

/**
 * @brief Translates a C-instruction into binary code.
 * 
 * C-instructions define computation and destination, with an optional jump condition. 
 * The binary format is "111a c1c2c3c4 c5c6d1d2 d3j1j2j3":
 * - `a` and `c (computation)`: Bits representing the computation type and operands.
 * - `d (destination)`: Specifies the destination register(s).
 * - `j (jump)`: Specifies a jump condition.
 * 
 * @param token The C-instruction in the format "dest=comp;jump".
 * @return string The 16-bit binary representation of the C-instruction.
 */
string translateCInstruction(const string& token){
    // Initial binary string for a C-instruction begins with "111"
    string binary = "111";

    // Parse dest, comp, and jump from instruction
    size_t equalSignPos = token.find('=');
    size_t semicolonPos = token.find(";");
    string dest, comp, jump;

    if(equalSignPos != string::npos){
        dest = token.substr(0, equalSignPos); // Destination before '='
    }

    if(semicolonPos != string::npos){
        jump = token.substr(semicolonPos + 1); // Jump after ';'
    }
    
    comp = token.substr(equalSignPos + 1, semicolonPos - equalSignPos - 1); // Computation in between

    binary += computationMap[comp] + destinationMap[dest] + jumpMap[jump];

    return binary;
}
