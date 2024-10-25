// Rect.asm
// Draws a rectangle at the top-left of the screen
// R0 determines the height of the rectangle
// Rectangle is 16 pixels wide

@R0       // Load R0 (height of the rectangle)
D=M       // Store R0 in D-register

@SCREEN   // Start of screen memory (address 16384)
@addr     // Label for screen memory address
M=A       // Initialize the screen memory address

(LOOP)    // Start of loop for each row
@addr     // Load screen memory address
A=M       // Set A-register to current address
M=-1      // Draw a row of 16 pixels (binary 1111111111111111)

@addr     // Load screen memory address
D=M       // Store current address in D-register
@32       // Move to the next row (screen width is 32)
D=D+A     // Increment address by 32 to move to the next row
@addr     // Load address into A-register
M=D       // Store updated address

@R0       // Decrement the height
D=M       // Load R0
D=D-1     // Subtract 1 from R0
@R0
M=D       // Store updated height in R0

@LOOP     // If height > 0, loop again
D;JGT

(END)     // End of program
@END
0;JMP
