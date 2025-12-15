//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

// Function definition for LookupSymbol
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {
    auto it = m_symbolTable.find(a_symbol);
    if (it != m_symbolTable.end()) {
        a_loc = it->second;
        return true;
    }
    return false;
}
/*
* NAME
*   DisplaySymbolTable - displays the symbol table.
* SYNOPSIS
*  void DisplaySymbolTable( );
*   DESCRIPTION
* 	 This function displays the contents of the symbol table to the standard output.
* */
void SymbolTable::DisplaySymbolTable()
{
    cout << "Symbol Table" << endl;
    cout << "------------" << endl;
    for (const auto& p : m_symbolTable) {
        cout << p.first << " : ";
        if (p.second == multiplyDefinedSymbol) {
            cout << "MULTIPLY_DEFINED";
        }
        else {
            cout << p.second;
        }
        cout << endl;
    }
    cout << endl;
}
