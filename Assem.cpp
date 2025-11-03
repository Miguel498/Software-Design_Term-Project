/*
 * Assembler main program.
 */
#include "stdafx.h"     // This must be present if you use precompiled headers which you will use. 
#include <stdio.h>

#include "Assembler.h"

int main( int argc, char *argv[] )
{
    Assembler assem( argc, argv );

    // Establish the location of the labels:
    assem.PassI( );

    // Display the symbol table.
    //MIGGY assem.DisplaySymbolTable();

    // Finish the translation and output it to the screen.
    //MIGGY assem.PassII( );
    
    // Run the emulator on the translation of the assembler language program that was generated in Pass II.
    //MIGGY assem.RunProgramInEmulator();
   
    // Terminate indicating all is well.  If there is an unrecoverable error, the 
    // program will terminate at the point that it occurred with an exit(1) call.
    return 0;
}