/**
 * @file Assembler.cpp
 * @brief Main entry point for the Assembler program.
 * 
 * This file contains the main function that serves as the entry point for the Assembler.
 * It prompts the user for an input file, initiates the parsing process, and handles any
 * potential errors during execution.
 * 
 * Key Components:
 * - User input handling for the assembly file name.
 * - Invocation of the parsing function to process the assembly code.
 * - Basic error handling for file operations.
 */
#include <iostream>
#include <fstream>
#include <string>
#include "../include/Parser.h"

using namespace std;

/**
 * @brief Main function for the Assembler program
 * 
 * This is the entry point of the assembler program. It prompts the user
 * to provide an assembly language source file name, then initiates the
 * parsing and translation process for converting assembly code to machine code.
 * 
 * Process Flow:
 * 1. Prompt user for a `.asm` file input.
 * 2. Call the `parse` function to start parsing and processing the file.
 * 3. Handle any exceptions that might occur during file processing.
 * 
 * @return int Returns 0 on successful execution, non-zero on error.
 */
int main(){
    string filename;
    cout<<"Enter the filename with extension ('filename.asm'): ";
    cin>>filename;

    try{
        // Initiate the parsing and processing of the provided assembly file
        parse(filename);
        cout<<"Assembly completed successfully."<<endl;
    }catch (const std::exception& e) {
        cerr<<"Error during assembly: "<<e.what()<<endl;
        return 1;  // Indicate error in program execution
    }

    return 0;
}
