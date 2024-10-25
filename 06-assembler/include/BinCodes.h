/**
 * @file BinCodes.h
 * @brief Header file for binary code translation in the Assembler.
 * 
 * This file declares functions for translating assembly instructions
 * into their binary machine code representations. It handles both A-instructions
 * and C-instructions.
 * 
 * Key Components:
 * - A-instruction translation: Converts "@value" format to 16-bit binary.
 * - C-instruction translation: Converts "dest=comp;jump" format to 16-bit binary.
 * 
 * The translation process is a core part of the assembler, converting human-readable
 * assembly code into machine-executable binary code.
 */
#ifndef BINCODES_H
#define BINCODES_H

#include <string>

/**
 * @brief Translates an A-instruction into binary code.
 * 
 * This function converts an A-instruction (e.g., "@123" or "@LOOP") into its
 * 16-bit binary representation. The binary format for A-instructions is:
 * 0vvv vvvv vvvv vvvv
 * Where 'v' represents the 15-bit value or address.
 * 
 * @param instruction The A-instruction in the format "@value" or "@symbol".
 * @return std::string The 16-bit binary string representation of the A-instruction.
 */
std::string translateAInstruction(const std::string& instruction);

/**
 * @brief Translates a C-instruction into binary code.
 * 
 * This function parses a C-instruction (e.g., "D=M+1;JGT") and generates its
 * 16-bit binary representation. The binary format for C-instructions is:
 * 111a cccc ccdd djjj
 * Where:
 * - 'a' is the comp bit (0 for A, 1 for M)
 * - 'c' represents the computation bits
 * - 'd' represents the destination bits
 * - 'j' represents the jump bits
 * 
 * @param instruction The C-instruction in "dest=comp;jump" format.
 * @return std::string The 16-bit binary representation of the C-instruction.
 */
std::string translateCInstruction(const std::string& instruction);

#endif // BINCODES_H
