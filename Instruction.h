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
        // Remove comments first
        a_line = RemoveComment(a_line);

        // Trim leading/trailing whitespace
        size_t start = a_line.find_first_not_of(" \t\r\n");
        if (start == string::npos) {
            // Blank or comment-only line
            m_Label.clear();
            m_OpCode.clear();
            m_Operand1.clear();
            m_Operand2.clear();
            m_instruction.clear();
            m_type = ST_Comment;
            m_IsNumericOperand = false;
            m_NumOpCode = 0;
            m_Operand1Value = 0;
            return m_type;
        }
        size_t end = a_line.find_last_not_of(" \t\r\n");
        string trimmed = a_line.substr(start, end - start + 1);
        m_instruction = trimmed;

        // Parse into components
        if (!ParseLine(trimmed, m_Label, m_OpCode, m_Operand1, m_Operand2)) {
            // Malformed line -> treat as comment to avoid crash (caller can record error)
            m_type = ST_Comment;
            return m_type;
        }

        // If no opcode found, it's a blank/comment line
        if (m_OpCode.empty()) {
            m_type = ST_Comment;
            return m_type;
        }

        // Normalize opcode to uppercase for comparisons (store original in m_OpCode if desired)
        string opc = m_OpCode;
        for (char& c : opc) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));

        // Check for END directive
        if (opc == "END") {
            m_type = ST_End;
            // keep opcode normalized
            m_OpCode = opc;
            return m_type;
        }

        // If opcode is purely numeric, treat as machine-language with numeric opcode
        bool opcodeIsNumeric = !opc.empty() && (opc.find_first_not_of("0123456789") == string::npos);
        if (opcodeIsNumeric) {
            try {
                m_NumOpCode = stoi(opc);
            }
            catch (...) {
                m_NumOpCode = 0;
            }
            m_type = ST_MachineLanguage;
        }
        else {
            m_type = ST_AssemblerInstr;
        }

        // Determine if operand1 is numeric (strip a trailing comma if present)
        string op1 = m_Operand1;
        if (!op1.empty() && op1.back() == ',') op1.pop_back();
        bool isNum = false;
        if (!op1.empty()) {
            // Accept optional leading +/-
            size_t pos = 0;
            if (op1[0] == '+' || op1[0] == '-') pos = 1;
            if (pos < op1.size() && op1.find_first_not_of("0123456789", pos) == string::npos) {
                try {
                    m_Operand1Value = stoi(op1);
                    isNum = true;
                }
                catch (...) {
                    isNum = false;
                }
            }
        }
        m_IsNumericOperand = isNum;

        // Keep opcode stored in normalized form for easier comparisons downstream
        m_OpCode = opc;

        return m_type;
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
    // --- New accessors to support Pass II translation ---

    // Get normalized opcode (upper-case or numeric string for machine language).
    inline string GetOpCode() const { return m_OpCode; }

    // Get operand text (operand1 may include trailing comma originally).
    inline string GetOperand1() const { return m_Operand1; }
    inline string GetOperand2() const { return m_Operand2; }

    // If opcode was numeric, get numeric opcode value.
    inline int GetNumOpCode() const { return m_NumOpCode; }

    // Operand1 numeric information (set during ParseInstruction)
    inline bool IsOperand1Numeric() const { return m_IsNumericOperand; }
    inline int GetOperand1Value() const { return m_Operand1Value; }

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
