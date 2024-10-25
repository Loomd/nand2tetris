/**
 * @file SymbolTables.h
 * @brief Header file for symbol table management in the Assembler.
 * 
 * This file declares functions and data structures for managing symbols
 * in the Hack assembly language. It includes support for predefined symbols,
 * user-defined labels, and variables.
 * 
 * Key Components:
 * - Symbol table: An unordered map storing symbol names and their addresses.
 * - Computation, destination, and jump maps: For C-instruction translation.
 * - Functions for adding and retrieving symbol addresses.
 * 
 * The symbol table is a crucial part of the assembler, allowing efficient
 * lookup and management of symbols throughout the assembly process.
 */
#ifndef SYMBOLTABLES_H
#define SYMBOLTABLES_H

#include <string>
#include <unordered_map>

/**
 * @brief Adds a new symbol to the symbol table.
 * 
 * This function is used to add labels and variables to the symbol table.
 * If the symbol already exists, it will not be added again.
 * 
 * @param symbol The name of the symbol to add.
 * @param address The memory address associated with the symbol.
 */
void addSymbol(const std::string& symbol, int address);

/**
 * @brief Retrieves the address of a symbol from the symbol table.
 * 
 * This function looks up a symbol in the table and returns its associated address.
 * If the symbol is not found, it returns -1.
 * 
 * @param symbol The name of the symbol to look up.
 * @return int The address of the symbol, or -1 if not found.
 */
int getSymbolAddress(const std::string& symbol);

// External declarations for symbol maps
extern std::unordered_map<std::string, int> symbolTable;
extern std::unordered_map<std::string, std::string> computationMap;
extern std::unordered_map<std::string, std::string> destinationMap;
extern std::unordered_map<std::string, std::string> jumpMap;

#endif // SYMBOLTABLES_H
