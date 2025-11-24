#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"
#include <sstream>
using namespace std;
//remove comments from a line 
//param name = "line"> The line to have its comment removed
//return> the original line with its comment removed
string Instruction::RemoveComment(string line) {
    size_t pos = line.find(';');
    if (pos == string::npos)
    {
        return line;
    }
    return line.erase(pos);
}

// Will parse a line into label, op code, and operand.
bool Instruction::ParseLine(const string& line, string& label, string& opcode, string& operand1, string &operand2)
{
    istringstream ins(line);
    label = opcode = operand1 = operand2 = "";
    if (line.empty()) return true;

    string extra;

    if (line[0] != ' ' && line[0] != '\t')
    {
        ins >> label;
    }
    ins >> opcode >> operand1 >> operand2 >> extra;

    return extra == "";

}
