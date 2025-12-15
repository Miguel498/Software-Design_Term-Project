/*
 * Assembler main program.
 */
#include "stdafx.h"
#include <stdio.h>

#include "Assembler.h"
#include "Errors.h"

int main(int argc, char* argv[])
{
    Assembler assem(argc, argv);

    // Initialize error reporting.
    Errors::InitErrorReporting();

    // Establish the location of the labels:
    assem.PassI();

    // Display the symbol table.
    assem.DisplaySymbolTable();

    // Finish the translation and output it to the screen.
    assem.PassII();

    // Run the emulator on the translation of the assembler language program that was generated in Pass II.
    assem.RunProgramInEmulator();

    // Display any errors that were recorded during assembly.
    if (Errors::HasErrors()) {
        cout << "Errors detected during assembly:" << endl;
        Errors::DisplayErrors();
    }

    // Terminate indicating all is well.  If there is an unrecoverable error, the 
    // program will terminate at the point that it occurred with an exit(1) call.
    return 0;
}
