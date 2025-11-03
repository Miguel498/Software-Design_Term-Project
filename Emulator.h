//
//		Emulator class - supports the emulation of VC1620 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H
#include <assert.h>

class emulator {

public:

    const static int MEMSZ = 100'000;	// The size of the memory of the VC1620.
    emulator() {

         m_memory.resize(MEMSZ, 0);
    }
    // Records instructions and data into simulated memory.
    bool insertMemory( int a_location, long long a_contents ){
        assert(a_location >= 0 && a_location < MEMSZ);
        m_memory[a_location] = a_contents;
      //  m_memory.at( a_location) = a_contents; //does bounce checking so we don't go out of the array
    }
    
    // Runs the program recorded in memory.
    bool runProgram( );

private:

    vector<long long> m_memory;  	// Memory for the VC1620
};

#endif
