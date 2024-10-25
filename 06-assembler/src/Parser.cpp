/**
 * @file Parser.cpp
 * @brief Implementation of the parser for the Assembler.
 * 
 * This file contains the main parsing logic for converting Hack assembly language
 * into machine code. It performs a two-pass parsing process to handle symbols and
 * translate instructions.
 * 
 * Key Components:
 * - Two-pass parsing algorithm
 * - Symbol table management
 * - Instruction translation (A-instructions and C-instructions)
 * 
 * The parser reads an input .asm file, processes it line by line, and outputs
 * the corresponding machine code to a .hack file.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include "../include/Parser.h"
#include "../include/SymbolTables.h"
#include "../include/BinCodes.h"

using namespace std;

/**
 * @brief Trims (removes) leading and trailing whitespace from a string.
 * 
 * This function eliminates extra spaces at the beginning and end of a line to ensure
 * consistent parsing and tokenization. Internal spaces are preserved to ensure tokens 
 * are not altered.
 * 
 * Key Components:
 * - `size_t`: A type used for non-negative indexing. It is commonly used for array indices
 *   and is an unsigned integer type that can vary depending on the system.
 * - `npos`: A constant representing "no position." It is returned when a search function
 *   (like `find_first_not_of`) does not find the specified character, signifying no match.
 * 
 * @param line The input string to be trimmed.
 * @return string A new string with leading and trailing whitespace removed.
 */
string trim(string line){
    size_t first = line.find_first_not_of(' ');
    if(first == string::npos) return "";  // If the string is all spaces, return an empty string
    size_t last = line.find_last_not_of(' ');
    return line.substr(first, last - first + 1);  // Extract and return the substring from first to last non-space
}

/**
 * @brief Parses the assembly file line-by-line, processing labels and translating instructions.
 * 
 * This function executes a two-pass process:
 * 
 * - First Pass: 
 *   1. Reads each line of the input file.
 *   2. Identifies and processes labels (e.g., `(LOOP)`).
 *   3. Stores labels in the symbol table with their corresponding ROM address.
 * 
 * - Second Pass:
 *   1. Rereads the file from the beginning.
 *   2. Processes A-instructions and C-instructions.
 *   3. Translates instructions into binary format.
 *   4. Handles variable symbols, assigning them to available RAM addresses.
 *   5. Writes the translated binary code to the output file.
 * 
 * Key Components:
 * - `ifstream`: Input file stream for reading the assembly file.
 * - `ofstream`: Output file stream for writing the machine code.
 * - `getline()`: Reads the file line-by-line.
 * - `seekg()`: Repositions the file pointer for the second pass.
 * 
 * @param filename The name of the input assembly file (.asm).
 */
void parse(string filename){
    string line;  // Holds each line of code from the file
    ifstream file(filename);  // Open the input file for reading
    string outputFilename = filename.substr(0, filename.find_last_of(".")) + ".hack";
    ofstream output(outputFilename);  // Open the output file for writing

    // Check if the input file opened successfully
    if(!file.is_open()){
        cerr<<"Error opening file: "<<filename<<endl;
        return;
    }

    /** 
     * First Pass: Add labels to the symbol table with ROM addresses.
     * 
     * This pass identifies labels within parentheses (e.g., `(LOOP)`). Labels represent 
     * memory addresses in the ROM, so each label is assigned the ROM address where it is 
     * encountered.
     * 
     * - `romAddress` represents the current position in ROM (line number of actual instructions).
     * - If a line is a label, we add it to `symbolTable` with the current `romAddress` and
     *   do not increment `romAddress`.
     */
    int romAddress = 0;  // Initialize the ROM address counter
    while(getline(file, line)){  // Read each line of the file
        size_t commentPosition = line.find("//");  // Find the start of any inline comment
        if(commentPosition != string::npos){
            line = line.substr(0, commentPosition);  // Remove comments by taking only the text before "//"
        }

        line = trim(line);  // Trim whitespace from the line

        if(line.empty()){
            continue;  // Skip empty lines
        }

        // Label Detection: A label is enclosed in parentheses, e.g., "(LOOP)"
        // If the first character of the string is '(' and the final is ')', after removing comments and whitespace, then it is a label.
        if(line[0] == '(' && line.back() == ')'){
            // Extract the label name by removing the parentheses
            // `substr(1, line.size() - 2)` takes a substring starting from the second character up to the second-to-last character.
            // This removes the outer parentheses from the label.
            string label = line.substr(1, line.size() - 2);  
            
            // `addSymbol` stores the label and its associated ROM address in `symbolTable`.
            // The label name is used as the key, and the current `romAddress` is the value. 
            // Since labels don't represent executable instructions, the address counter is not incremented.
            addSymbol(label, romAddress);
        }else{
            // `romAddress` is only incremented for actual instructions, not labels.
            // This ensures that labels themselves don’t take up memory addresses in ROM.
            romAddress++;
        }
    }

    /**
     * File Handling for the Second Pass:
     * - The file has an "internal pointer" that tracks where reading is occurring. 
     *   Once the first pass completes, this pointer is at the end of the file.
     * 
     * - `seekg(0, ios::beg)`: The `seekg` function moves the file pointer back to a 
     *   specific position. The arguments here, `0` and `ios::beg`, tell the file to 
     *   start reading again from the beginning.
     *     - `ios::beg` represents the beginning of the file, allowing a full re-read.
     * 
     * - `clear()`: This function clears any flags that were set during the first read 
     *   (like the end-of-file flag, which would prevent further reading).
     */
    file.clear();
    file.seekg(0, ios::beg);  // Reset file position for the second pass

    /**
     * Second Pass: Process A-instructions and C-instructions, resolve symbols, and translate.
     * 
     * - `nextAvailableRamAddress` starts at 16 because the Hack assembly reserves
     *   addresses 0–15 for predefined symbols. User-defined symbols start at RAM[16].
     * - 'anotherInstruction' Tracks if there's another instruction; newline control.
     */
    int nextAvailableRamAddress = 16;
    bool anotherInstruction = true;

    while(getline(file, line)){  // Read each line again from the beginning
        size_t commentPosition = line.find("//");  // Find the start of any comment
        if(commentPosition != string::npos){
            line = line.substr(0, commentPosition);  // Strip the comment
        }

        line = trim(line);  // Remove leading/trailing spaces

        // Skip empty lines and labels (labels were processed in the first pass)
        if(line.empty() || (line[0] == '(' && line.back() == ')')){
            continue;
        }

        // For each instruction after the first, add a newline before writing
        if(!anotherInstruction){
            output<<endl;
        }
        anotherInstruction = false; // After first line, subsequent lines will start with a newline

        // Process A-instructions (e.g., "@2" or "@LOOP")
        if(line[0] == '@'){
            string symbol = line.substr(1);  // Extract the symbol after '@'
            int address;
            if(isdigit(symbol[0])){  // If the symbol is a number, directly convert to an integer
                address = stoi(symbol);  // Convert numeric address to integer
            }else{  // Otherwise, it's a user-defined symbol or predefined variable
                address = getSymbolAddress(symbol);  // Look up the symbol in the symbol table
                if(address == -1){  // If symbol is not found in the table
                    address = nextAvailableRamAddress++;  // Assign it the next available RAM address
                    addSymbol(symbol, address);  // Add the new symbol to the symbol table
                }
            }
            output<<translateAInstruction("@" + to_string(address));  // Translate A-instruction

        }else{
            // Process C-instructions (e.g., "D=M+1")
            output<<translateCInstruction(line);  // Translate C-instruction
        }
    }

    // Close input and output files
    file.close();
    output.close();
}
