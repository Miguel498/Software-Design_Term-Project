//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End                  // end instruction.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction( string a_line )
    {
	    a_line = RemoveComment(a_line);
	    if(ParseLine(a_line, m_Label, m_OpCode, m_Operand1, m_Operand2)){
		    //???
	    };
	    return ST_Comment; //MIGGY 
    }
    bool ParseLine(const string& line, string& label, string& opcode, 
    string& operand1, string &operand2);


    // Compute the location of the next instruction.
	int LocationNextInstruction( int a_loc )
	{
	    return a_loc++; //MIGGY 
    };
    // To access the label
    inline string &GetLabel( ) {

        return m_Label;
    };
    // To determine if a label is blank.
    inline bool isLabel( ) {

        return ! m_Label.empty();
    };


private:{
    string RemoveComment(string line);
    bool ParseLine(const string& line, string& label, string& opcode, string& operand1, string &operand2);

    };

    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;      // The operands
    string m_Operand2;      

    string m_instruction;  // The original instruction.

    // Derived values.
    int m_NumOpCode;       // The numerical value of the op code for machine language equivalents.
    InstructionType m_type;// The type of instruction.

    bool m_IsNumericOperand;// == true if the operand is numeric.
    int m_Operan1Value;   // The value of the operand if it is numeric.

};
