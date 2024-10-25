// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// This program runs an infinite loop that listens to keyboard input.
// When a key is pressed, the program fills the screen with black pixels (sets each pixel to 1).
// When no key is pressed, the screen is cleared (sets each pixel to 0).

(LOOP)
    @KBD        // Load the value from the keyboard register (24576) into the D-register
    D=M         // Copy the value of the keyboard input into the D-register
    
    @FILL       // If the value in D is greater than 0 (any key is pressed), jump to FILL
    D;JGT       // Jump if D > 0

    @CLEAR      // Otherwise, if no key is pressed, jump to CLEAR to clear the screen
    D;JEQ       // Jump if D == 0

(FILL)          // This label will blacken the screen when any key is pressed
    @R0         // Load the register R0 to store the value for filling the screen (black pixels)
    M=-1        // Set R0 to -1, which means all bits are set (blacken the screen)
    @DRAW       // Jump to DRAW to iterate through the screen and fill it
    0;JMP       // Unconditional jump to DRAW

(CLEAR)         // This label will clear the screen when no key is pressed
    @R0         // Load the register R0 to store the value for clearing the screen
    M=0         // Set R0 to 0, meaning the screen will be cleared (all pixels off)
    @DRAW       // Jump to DRAW to iterate through the screen and clear it
    0;JMP       // Unconditional jump to DRAW

(DRAW)          // This label is responsible for writing the current value of R0 to all screen pixels
    @8191       // Load 8191 into the A-register (number of screen words minus 1)
    D=A         // Copy the value 8191 into the D-register (this will be our counter)
    @R1         // Load the R1 register to store the counter value
    M=D         // Store 8191 (counter value) in R1

    @SCREEN     // Load the base address of the screen (16384) into the A-register
    D=A         // Copy the screen base address into the D-register
    @addr       // Load the addr variable to store the current screen address
    M=D         // Store the screen base address in addr

(NEXT)          // Loop through all screen addresses
    @R0         // Load the value in R0 (either 0 or -1) into the D-register
    D=M
    @addr       // Load the current screen address from addr
    A=M         // Set the A-register to the current address
    M=D         // Write the value from R0 (either 0 or -1) to the current screen address

    @addr       // Load the addr variable again to increment the address
    D=M
    D=D+1       // Increment the address by 1
    M=D         // Store the incremented address back in addr

    @R1         // Load the R1 register to decrement the counter
    D=M
    D=D-1       // Decrement the counter by 1
    M=D         // Store the decremented value back into R1

    @NEXT       // If the counter is greater than or equal to 0, continue the loop
    D;JGE       // Jump if D >= 0

@LOOP           // After drawing is complete, jump back to the start to check for new keyboard input
0;JMP           // Infinite loop to continuously check the keyboard status