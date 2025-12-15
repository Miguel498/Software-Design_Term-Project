 #include "stdafx.h"
 #include "Emulator.h"
 
bool emulator::runProgram() {
    int loc = 100;
    const int maxSteps = 1'000'000; // safety to avoid infinite loops in simple tests
    int steps = 0;

    // Basic execution loop for simple tests:
    // - Extract opcode/address fields
    // - Stop when opcode == 0 
    while (loc >= 0 && loc < MEMSZ && steps < maxSteps) {
        long long inst = m_memory[loc]; // copy from memory

        int opcode = static_cast<int>(inst / 10'000'000'000LL); // top digits = opcode
        int address1 = static_cast<int>((inst / 100'000) % 10'000); // first address
        int address2 = static_cast<int>(inst % 100'000); // second address

        // For now, treat opcode 0 as HALT/END for simple tests.
        if (opcode == 0) {
            return true;
        }

        // No real instruction execution implemented yet.
        // Advance to next instruction to allow tests to complete.
        ++loc;
        ++steps;
    }

    // If we exit the loop because of bounds or step limit, indicate failure.
    return false;
}
