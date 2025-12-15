//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{
}
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );
        Errors::RecordError("MIGGY: In Pass1");
        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::ST_Comment )  
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
// Pass II generates the machine language translation of the source code.
// Pass II - generate a translation
void Assembler::PassII()
{
    // Reset file to beginning and translate into emulator memory.
    m_facc.rewind();

    int loc = 0;

    string line;
    while (m_facc.GetNextLine(line)) {

        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If end directive, stop processing.
        if (st == Instruction::ST_End) {
            break;
        }

        // Skip comments and blank lines.
        if (st == Instruction::ST_Comment) continue;

        // Handle assembler directives (simple support for ORG, DC, DS).
        if (st == Instruction::ST_AssemblerInstr) {
            string opc = m_inst.GetOpCode();

            if (opc == "ORG") {
                // Set location counter.
                if (m_inst.IsOperand1Numeric()) {
                    loc = m_inst.GetOperand1Value();
                }
                else {
                    int symLoc;
                    if (m_symtab.LookupSymbol(m_inst.GetOperand1(), symLoc)) {
                        loc = symLoc;
                    }
                    else {
                        Errors::RecordError("Undefined symbol in ORG: " + m_inst.GetOperand1());
                    }
                }
                continue;
            }
            else if (opc == "DC") {
                long long value = 0;
                if (m_inst.IsOperand1Numeric()) {
                    value = m_inst.GetOperand1Value();
                }
                else {
                    int symLoc;
                    if (m_symtab.LookupSymbol(m_inst.GetOperand1(), symLoc)) {
                        value = symLoc;
                    }
                    else {
                        Errors::RecordError("Undefined symbol in DC: " + m_inst.GetOperand1());
                    }
                }
                // Store value into emulator memory at loc.
                if (!m_emul.insertMemory(loc, value)) {
                    Errors::RecordError("Memory insert failed at location " + to_string(loc));
                }
                loc = m_inst.LocationNextInstruction(loc);
                continue;
            }
            else if (opc == "DS") {
                int count = 0;
                if (m_inst.IsOperand1Numeric()) {
                    count = m_inst.GetOperand1Value();
                }
                else {
                    int symLoc;
                    if (m_symtab.LookupSymbol(m_inst.GetOperand1(), symLoc)) {
                        count = symLoc;
                    }
                    else {
                        Errors::RecordError("Undefined symbol in DS: " + m_inst.GetOperand1());
                    }
                }
                for (int i = 0; i < count; ++i) {
                    if (!m_emul.insertMemory(loc + i, 0LL)) {
                        Errors::RecordError("Memory insert failed at location " + to_string(loc + i));
                    }
                }
                loc += count;
                continue;
            }
            else {
                // Unknown assembler directive - record error and continue.
                Errors::RecordError("Unknown assembler directive: " + opc);
                continue;
            }
        }

        // Machine language instruction
        if (st == Instruction::ST_MachineLanguage) {
            int opcode = m_inst.GetNumOpCode();
            int address1 = 0;

            // Operand1 may be numeric or a symbol.
            if (m_inst.IsOperand1Numeric()) {
                address1 = m_inst.GetOperand1Value();
            }
            else {
                string op1 = m_inst.GetOperand1();
                // strip trailing comma if present
                if (!op1.empty() && op1.back() == ',') op1.pop_back();
                int symLoc;
                if (op1.empty()) {
                    address1 = 0;
                }
                else if (m_symtab.LookupSymbol(op1, symLoc)) {
                    address1 = symLoc;
                }
                else {
                    Errors::RecordError("Undefined symbol in machine instruction operand: " + op1);
                }
            }

            // Encode instruction: opcode * 10^10 + address1 * 10^5 + address2(=0)
            long long word = static_cast<long long>(opcode) * 10'000'000'000LL
                + static_cast<long long>(address1) * 100'000LL
                + 0LL;

            if (!m_emul.insertMemory(loc, word)) {
                Errors::RecordError("Memory insert failed at location " + to_string(loc));
            }

            loc = m_inst.LocationNextInstruction(loc);
            continue;
        }
    }
}
// Run emulator on the translation.
void Assembler::RunProgramInEmulator()
{
    // Attempt to run the program recorded in the emulator's memory.
    if (!m_emul.runProgram()) {
        // Record error so it appears with other assembler diagnostics.
        Errors::RecordError("Runtime error: emulator failed to run program.");
    }
}


