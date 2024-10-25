/**
 * @file Parser.h
 * @brief Header file for the Assembler parser.
 * 
 * This file declares the main parsing function used in the Assembler.
 * The parser is responsible for reading the assembly language file (.asm),
 * processing its contents, and generating the corresponding machine code.
 * 
 * Key Components:
 * - parse function: The main entry point for the parsing process.
 * 
 * The parser implements a two-pass algorithm:
 * 1. First pass: Identify and process labels, building the symbol table.
 * 2. Second pass: Translate instructions to binary, handling variables and symbols.
 */
#ifndef PARSER_H
#define PARSER_H

#include <string>

/**
 * @brief Parses the given assembly file line by line.
 * 
 * This function is the core of the assembler, responsible for:
 * 1. Reading the input .asm file.
 * 2. Processing labels and building the symbol table.
 * 3. Translating A-instructions and C-instructions to binary.
 * 4. Handling variable symbols and assigning memory addresses.
 * 5. Writing the resulting machine code to a .hack file.
 * 
 * The function utilizes helper functions from `SymbolTables` and `BinCodes`
 * for symbol handling and instruction translation.
 * 
 * @param filename The name of the input assembly file (.asm).
 */
void parse(std::string filename);

#endif // PARSER_H
