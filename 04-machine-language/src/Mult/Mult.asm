// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// This program multiplies the values in R0 and R1 and stores the result in R2.
// The algorithm is based on repetitive addition, meaning R0 is added to itself R1 times.

@R0    // Load the value from RAM[0] (the first multiplicand) into the A-register
D=M    // Store the value of RAM[0] in the D-register
@STOP  // If RAM[0] is 0, jump to the STOP label to terminate
D;JEQ  // Jump if D == 0

@num1  // Store the value of RAM[0] in the variable num1
M=D    // Store the value of D in num1

@R1    // Load the value from RAM[1] (the second multiplicand) into the A-register
D=M    // Store the value of RAM[1] in the D-register
@STOP  // If RAM[1] is 0, jump to the STOP label to terminate
D;JEQ  // Jump if D == 0

@num2  // Store the value of RAM[1] in the variable num2
M=D    // Store the value of D in num2

@sum   // Initialize the sum variable to 0
M=0    // Store 0 in sum (this will hold the result of the multiplication)

(LOOP) // Loop to perform the repetitive addition
    @num1  // Load the value of num1 into the A-register
    D=M    // Store the value of num1 in the D-register

    @sum   // Load the sum variable
    D=D+M  // Add the value of num1 to the current value of sum
    M=D    // Store the updated sum

    @num2  // Load the value of num2
    D=M    // Store the value of num2 in the D-register
    D=D-1  // Decrement num2 by 1
    M=D    // Store the decremented value back in num2

    @STOP  // If num2 is now 0, jump to STOP to end the loop
    D;JEQ  // Jump if D == 0

    @LOOP  // Otherwise, jump back to the start of the loop
    D;JGT  // Jump if D > 0

(STOP) // Prepare to store the result and terminate
    @sum   // Load the final sum value into the A-register
    D=M    // Store the value of sum in the D-register

    @R2    // Load RAM[2] (where the result will be stored)
    M=D    // Store the value of sum into RAM[2]

    @END   // Jump to END to terminate the program
    0;JMP  // Infinite loop to prevent further execution

(END)  // Infinite loop to terminate the program
    @END   // Load the address of the END label
    0;JMP  // Infinite loop to prevent further execution
