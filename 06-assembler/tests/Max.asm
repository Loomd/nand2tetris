// Max.asm
// Computes max(R0, R1) and stores the result in R2

@R0      // Load value from R0
D=M      // Store value of R0 in D-register
@R1      // Load value from R1
D=D-M    // Subtract R1 from R0, result in D-register

@R0GT    // If R0 > R1, jump to R0GT
D;JGT

@R1      // Load value from R1
D=M      // Store value of R1 in D-register
@R2      // Address register R2
M=D      // Store the max (R1) in R2
@END     // Jump to END
0;JMP

(R0GT)   // R0 is greater, so store R0 in R2
@R0      // Load value from R0
D=M      // Store value of R0 in D-register
@R2      // Address register R2
M=D      // Store the max (R0) in R2

(END)    // End of the program
@END
0;JMP
