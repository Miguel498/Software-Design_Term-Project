//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/*
NAME

    Assembler - constructor for the Assembler class.

SYNOPSIS

    Assembler::Assembler(int argc, char *argv[]);

DESCRIPTION

    Initializes the assembler by opening the source file through
    the FileAccess object.
*/
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv)
{
    // Nothing else to do here at this point.
}

/*
NAME

    ~Assembler - destructor for the Assembler class.

DESCRIPTION

    Currently does nothing. May need additions as project develops.
*/
Assembler::~Assembler()
{
}

/*
NAME

    PassI - first pass of the assembler.

SYNOPSIS

    void Assembler::PassI();

DESCRIPTION

    Establishes the location of all labels. Reads each line of
    source code, parses it, and adds any labels to the symbol table
    along with their locations.
*/
void Assembler::PassI()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (; ; ) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::ST_End) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if (st == Instruction::ST_Comment)
        {
            continue;
        }

        // Handle ORG directive - sets the location counter
        if (st == Instruction::ST_AssemblerInstr && m_inst.GetOpCode() == "ORG") {
            if (m_inst.IsOperand1Numeric()) {
                loc = m_inst.GetOperand1Value();
            }
            continue;
        }

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {
            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}

/*
NAME

    PassII - second pass of the assembler.

SYNOPSIS

    void Assembler::PassII();

DESCRIPTION

    Generates the machine language translation of the source code.
    Displays the translation in the format specified by the assignment.
*/
void Assembler::PassII()
{
    // Reset file to beginning.
    m_facc.rewind();

    int loc = 0;

    // Print header for translation output
    cout << "Translation of Program:" << endl;
    cout << endl;
    cout << "Location    Contents    Original Statement" << endl;

    string line;
    while (m_facc.GetNextLine(line)) {

        Instruction::InstructionType st = m_inst.ParseInstruction(line);
        string origStatement = m_inst.GetOriginalStatement();

        // If end directive, print it and stop processing.
        if (st == Instruction::ST_End) {
            // Print the end statement without location/contents
            cout << "                                     " << origStatement << endl;
            break;
        }

        // Handle comments - just print the original statement
        if (st == Instruction::ST_Comment) {
            cout << "                                     " << origStatement << endl;
            continue;
        }

        // Handle assembler directives
        if (st == Instruction::ST_AssemblerInstr) {
            string opc = m_inst.GetOpCode();

            if (opc == "ORG") {
                // Print ORG statement
                cout << setw(5) << right << loc << "                              " << origStatement << endl;

                // Set location counter
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

                // Print DC statement with contents
                cout << setw(5) << right << loc << "          ";
                cout << setw(12) << setfill('0') << right << value << setfill(' ');
                cout << "   " << origStatement << endl;

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

                // Initialize reserved memory to 0
                for (int i = 0; i < count; ++i) {
                    if (!m_emul.insertMemory(loc + i, 0LL)) {
                        Errors::RecordError("Memory insert failed at location " + to_string(loc + i));
                    }
                }

                // Print DS statement without contents
                cout << setw(5) << right << loc << "                     " << origStatement << endl;

                loc += count;
                continue;
            }
            else {
                // Unknown assembler directive - record error
                Errors::RecordError("Unknown assembler directive: " + opc);
                continue;
            }
        }

        // Machine language instruction
        if (st == Instruction::ST_MachineLanguage) {
            int opcode = m_inst.GetNumOpCode();
            int address1 = 0;
            int address2 = 0;

            // Operand1 may be numeric or a symbol
            string op1 = m_inst.GetOperand1();
            if (m_inst.IsOperand1Numeric()) {
                address1 = m_inst.GetOperand1Value();
            }
            else if (!op1.empty()) {
                int symLoc;
                if (m_symtab.LookupSymbol(op1, symLoc)) {
                    address1 = symLoc;
                }
                else {
                    Errors::RecordError("Undefined symbol: " + op1);
                }
            }

            // Operand2 may be numeric or a symbol
            string op2 = m_inst.GetOperand2();
            if (m_inst.IsOperand2Numeric()) {
                address2 = m_inst.GetOperand2Value();
            }
            else if (!op2.empty()) {
                int symLoc;
                if (m_symtab.LookupSymbol(op2, symLoc)) {
                    address2 = symLoc;
                }
                else {
                    Errors::RecordError("Undefined symbol: " + op2);
                }
            }

            // Encode instruction: opcode * 10^10 + address1 * 10^5 + address2
            long long word = static_cast<long long>(opcode) * 10'000'000'000LL
                + static_cast<long long>(address1) * 100'000LL
                + static_cast<long long>(address2);

            if (!m_emul.insertMemory(loc, word)) {
                Errors::RecordError("Memory insert failed at location " + to_string(loc));
            }

            // Print machine language instruction
            cout << setw(5) << right << loc << "          ";
            cout << setw(12) << setfill('0') << right << word << setfill(' ');
            cout << "  " << origStatement << endl;

            loc = m_inst.LocationNextInstruction(loc);
            continue;
        }
    }

    // Print separator and wait for user
    cout << endl;
    cout << "___________________________________________________________" << endl;
    cout << endl;
    cout << "Press Enter to continue...";
    cin.get();
    cout << endl;
}

/*
NAME

    RunProgramInEmulator - runs the assembled program.

SYNOPSIS

    void Assembler::RunProgramInEmulator();

DESCRIPTION

    Executes the program that was loaded into the emulator's memory
    during Pass II.
*/
void Assembler::RunProgramInEmulator()
{
    // Attempt to run the program recorded in the emulator's memory.
    if (!m_emul.runProgram()) {
        // Record error so it appears with other assembler diagnostics.
        Errors::RecordError("Runtime error: emulator failed to run program.");
    }

    cout << endl;
    cout << endl;
}
