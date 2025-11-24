 #include "stdafx.h"
 #include "Emulator.h"
 
 bool emulator::runProgram( ){
        int loc = 100;
        while(true){
            long long inst = m_memory[loc];//copy in memory

            int opcode = (int) (inst / 10'000'000'000LL); // to show the op code
            int address1 = (inst / 100'000) % 10'000; // to show the first address 
            int address2 = inst % 100'000; // to show the second address

            switch( opcode )
             {
              case 1: //add
                 m_memory[address1] = += m_memory[address2];
                 loc++;
              case 2: //-=

              default:
                 // Error
             }
         
        }
    }
