//
// Class to parse and provide information about instructions.
// Note: you will be adding more functionality.
//
#pragma once
#include <string>
using namespace std;    

// The elements of an instruction.
class Instruction {

public:
    Instruction() = default;
    ~Instruction() = default;

    // Codes to indicate the type of instruction we are processing.
    // (Later, this should be changed to an enum class.)
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End                  // End instruction.
    };

    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line)
    {
        a_line = RemoveComment(a_line);

        if (ParseLine(a_line, m_Label, m_OpCode, m_Operand1, m_Operand2)) {
            // TODO: Determine and return correct type
            return ST_AssemblerInstr;
        }

        return ST_Comment; 
    }

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc)
    {
        return ++a_loc;
    }

    // Access the label.
    inline string& GetLabel() {
        return m_Label;
    }

    // Determine if a label exists.
    inline bool isLabel() const {
        return !m_Label.empty();
    }

private:
    string RemoveComment(string line);
    bool ParseLine(const string& line, string& label, string& opcode, string& operand1, string& operand2);

    // The elements of an instruction.
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic opcode.
    string m_Operand1;     // The first operand.
    string m_Operand2;     // The second operand.

    string m_instruction;  // The original instruction.

    // Derived values.
    int m_NumOpCode = 0;               // Numeric opcode value.
    InstructionType m_type = ST_Comment; // Default type.
    bool m_IsNumericOperand = false;     // True if operand is numeric.
    int m_Operand1Value = 0;             // Numeric operand value if numeric.
};
