// Add.asm
// Adds 2 + 3 and stores the result in R0

@2       // Load constant 2 into A-register
D=A      // Store 2 in D-register
@R0      // Address register R0
M=D      // Store 2 in R0

@3       // Load constant 3 into A-register
D=A      // Store 3 in D-register
@R0      // Address register R0
M=M+D    // Add 3 to the value in R0 (which is 2), result stored in R0

(END)
0;JMP    // Infinite loop to keep the program running
