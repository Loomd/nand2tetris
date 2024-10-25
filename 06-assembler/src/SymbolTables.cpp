/** 
 *  @file SymbolTables.cpp
 *  @brief This module is responsible for managing symbols during the assembly translation process.
 *  It handles both predefined symbols (e.g., "R0", "SCREEN") and user-defined symbols (e.g., labels and variables).
 *   
 *  Overview:
 *  - `symbolTable` is a global unordered map storing symbol names as keys and memory addresses as values.
 *  - `addSymbol` function allows adding new symbols with associated addresses to the map.
 *  - `getSymbolAddress` function retrieves the address for a given symbol.
 *  
 *  Key Concepts:
 *  - `unordered_map`: A hash table-based data structure that provides fast lookup times. 
 *                     Each symbol maps to a unique memory address.
 *  - `auto`: A keyword used for automatic type deduction, especially useful for iterators.
 */
#include "../include/SymbolTables.h"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

/*
    `unordered_map<string, int> symbolTable`
    This unordered map acts as a symbol table, where each symbol (string) maps to an address (int).
    
    - Predefined symbols (e.g., "SP", "LCL", "ARG") are initialized with fixed addresses.
    - This map uses hashing for quick lookup, making it efficient for frequent additions and retrievals.
    
    Symbols are initialized here to ensure that the assembler can reference standard symbols immediately.
    New symbols (like labels or variables) can be added dynamically during parsing.
*/
unordered_map<string, int>symbolTable={
    {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4},
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5},
    {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11},
    {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
    {"SCREEN", 16384}, {"KBD", 24576}
};

// Predefined mappings for C-instruction fields
unordered_map<string, string>computationMap={
    {"0"  , "0101010"}, {"1"  , "0111111"}, {"-1" , "0111010"},
    {"D"  , "0001100"}, {"A"  , "0110000"}, {"M"  , "1110000"},
    {"!D" , "0001101"}, {"!A" , "0110001"}, {"!M" , "1110001"},
    {"-D" , "0001111"}, {"-A" , "0110011"}, {"-M" , "1110011"},
    {"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"},
    {"D-1", "0001110"}, {"A-1", "0110010"}, {"M-1", "1110010"},
    {"D+A", "0000010"}, {"D+M", "1000010"}, {"D-A", "0010011"},
    {"D-M", "1010011"}, {"A-D", "0000111"}, {"M-D", "1000111"},
    {"D&A", "0000000"}, {"D&M", "1000000"}, {"D|A", "0010101"},
    {"D|M", "1010101"}
};

unordered_map<string, string>destinationMap={
    {"" , "000"}, {"M" , "001"}, {"D" , "010"}, {"MD" , "011"},
    {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}
};

unordered_map<string, string>jumpMap={
    {""   , "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
    {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}
};

/*
    addSymbol(string& symbol, int address)
    Adds a new symbol with its associated address to the `symbolTable` if the symbol does not already exist.
    
    Parameters:
    - `symbol`: A reference to the string representing the symbol's name.
    - `address`: An integer representing the memory address to associate with the symbol.
    
    Logic:
    - `symbolTable.find(symbol)`: Checks if the symbol already exists in the table.
    - If not found, the symbol and its address are added to the map.
    
    Concept Note:
    - `unordered_map::find` returns an iterator pointing to the element if found, or to `unordered_map::end` if not.
    - Passing `symbol` by reference (`string&`) improves efficiency by avoiding copying.
*/
void addSymbol(const string& symbol, int address){
    if(symbolTable.find(symbol) == symbolTable.end()){
        symbolTable[symbol] = address;
    }
}

/*
    getSymbolAddress(string& symbol)
    Retrieves the memory address associated with a given symbol. 
    Returns -1 if the symbol is not found in `symbolTable`.
    
    Parameters:
    - `symbol`: A reference to the string representing the symbol's name.

    Returns:
    - `int`: The address associated with the symbol if found; otherwise, returns -1.
    
    Logic:
    - `auto it = symbolTable.find(symbol)`: Uses `auto` to deduce the type of `it`, which is an iterator for `unordered_map`.
    - `it->second`: If found, `it->second` provides the address (value) mapped to the symbol (key).
    - If `it` is at the end of the map (`unordered_map::end`), the symbol was not found, so return -1.
*/
int getSymbolAddress(const string& symbol){
    auto it = symbolTable.find(symbol);  // `auto` deduces type `unordered_map<string, int>::iterator`
    if(it != symbolTable.end()){
        return it->second;  // Access the mapped address if the symbol exists
    }else{
        return -1;  // Symbol not found
    }
}
