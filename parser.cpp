#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "scanner.cpp"
#include "translator.h"

using namespace std;

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: **
//=================================================

// ----- Utility Functions and Globals -----------------------------------

tokentype lookahead = ERROR;
string next_word;

// Type of error: SYNTAX ERROR: expected <token> but found <token>
// Done by: Ahmed Ali 
void syntaxerror1(tokentype expected){
    cout << "SYNTAX ERROR: expected " << tokenName[expected] 
         << " but found " << tokenName[lookahead] << endl;
    exit(1);
}

// Type of error: SYNTAX ERROR: unexpected <token> found in <non-terminal>
// Done by: Ahmed Ali 
void syntaxerror2(tokentype tt, string nonterminal) {
    cout << "SYNTAX ERROR: unexpected " << tokenName[tt]
         << " found in " << nonterminal << endl;
    exit(1);
}


void scan() {
    saved_J_word = next_word;
    saved_lexme = lookahead;
    scanner(lookahead, next_word);
    // cout << "Scanner called using word: " << word << endl;
    // cout << "Lookahead is: " << tokenName[lookahead] << endl;
}

// Purpose: Returns the next token. Does not match against it or advance the input.
// Done by: Nathan Hawrysh
tokentype next_token(){
    return lookahead;
}

// Purpose: Matches `expected` against the next token and advances the input by one token.
// Done by: Nathan Hawrysh
void match(tokentype expected) {
    if (lookahead != expected) {
        syntaxerror1(expected);
    }
    cout << "Matched " << tokenName[expected] << endl;
    scan();
}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

// Grammar: **
// Done by: Nathan Hawrysh & Kyle Cayne

void story();
void s();
void noun();
void verb();
void tense();
void be();
void after_subject();
void after_noun();
void after_destination();
void after_object();

// Grammar: s {s}
// Done by: Nathan Hawrysh
void story() {
    cout << "Processing <story>" << endl;
    do { s(); } while (next_token() != EOFM);
    cout << "\nSuccessfully parsed <story>." << endl;
}

// Grammar: [CONNECTOR] <noun> SUBJECT <after_subject>
// Done by: Nathan Hawrysh
void s() {
    cout << "Processing <s>" << endl;
    if (next_token() == CONNECTOR){ 
          match(CONNECTOR);
          // Added by Eric Tyler
          getEword();
          gen("CONNECTOR");
    }
    
    noun();
    getEword();
    
    match(SUBJECT);
    gen("ACTOR");
    
    after_subject();
}

// Grammar: WORD1 | PRONOUN
// Done by: Kyle Cayne
void noun() {
    cout << "Processing <noun>" << endl;
    tokentype next = next_token();
    if (next == WORD1) { match(WORD1); }
    else if (next == PRONOUN) { match(PRONOUN); }
    else { syntaxerror2(next, "<noun>"); }
}

// Grammar: WORD2
// Done by: Kyle Cayne
void verb() {
    cout << "Processing <verb>" << endl;
    match(WORD2);
}

// Grammar: VERBPAST  | VERBPASTNEG | VERB | VERBNEG
// Done by: Nathan Hawrysh
void tense() {
    cout << "Processing <tense>" << endl;
    tokentype next = next_token();
    switch (next) {
        case VERBPAST:
            match(VERBPAST);
            break;
        case VERBPASTNEG:
            match(VERBPASTNEG);
            break;
        case VERB:
            match(VERB);
            break;
        case VERBNEG:
            match(VERBNEG);
            break;
        default:
            syntaxerror2(next, "<tense>");
    }
}

// Grammar: IS | WAS
// Done by: Nathan Hawrysh
void be() {
    cout << "Processing <be>" << endl;
    tokentype next = next_token();
    switch (next) {
        case IS:
            match(IS);
            break;
        case WAS:
            match(WAS);
            break;
        default:
            syntaxerror2(next, "<be>");
    }
}

// Grammar: <verb> <tense> PERIOD | <noun> <after noun>
// Done by: Nathan Hawrysh
// Modified by: Eric Tyler and Nathan Hawrysh
void after_subject(){
    cout << "Processing <after subject>" << endl;
    tokentype next = next_token();
    if (next == WORD2) // <s> case 1
    {
        verb();
        getEword();
        gen("ACTION");
        tense();
        gen("TENSE");
        match(PERIOD);
    }
    else if (next == WORD1 || next == PRONOUN) { // <s> cases 2–5
        noun();
        getEword();
        after_noun();
    }
    else { syntaxerror2(next, "<after subject>"); }
}

// Grammar: <be> PERIOD | DESTINATION <after destination> | OBJECT <after object>
// Done by: Kyle Cayne & Nathan Hawrysh
// Modified by: Kyle Cayne & Nathan Hawrysh
void after_noun(){
    cout << "Processing <after noun>" << endl;
    tokentype next = next_token();
    switch (next) {
        case IS: case WAS: // <s> case 2
            be();
            gen("DESCRIPTION");
            gen("TENSE");
            match(PERIOD);
            break;
        case DESTINATION:
            match(DESTINATION);
            gen("TO");
            after_destination();
            break;
        case OBJECT:
            match(OBJECT);
            gen("OBJECT");
            after_object();
            break;
        default:
            syntaxerror2(next, "<after noun>");
    }
}

// Grammar: <verb> <tense> PERIOD
// Done by: Kyle Cayne & Nathan Hawrysh
// Modified by: Nathan Hawrysh
void after_destination() {
    cout << "Processing <after destination>" << endl;
    verb();
    getEword();
    gen("ACTION");
    tense();
    gen("TENSE");
    match(PERIOD);
}

// Grammar: <after destination> | <noun> DESTINATION <after destination>
// Done by: Kyle Cayne & Nathan Hawrysh
void after_object() {
    cout << "Processing <after object>" << endl;
    tokentype next = next_token();
    if (next == WORD2) {
        after_destination();
    }
    else if (next == WORD1 || next == PRONOUN) { // <s> case 5
        noun();
        getEword();
        match(DESTINATION);
        gen("TO");
        after_destination();
    }
    else { syntaxerror2(next, "<after object>"); }
}

string filename;

//----------- Driver ---------------------------

/*
// The new test driver to start the parser
// Done by: One of us from the future
int main()
{
    cout << "Enter the input file name: ";
    cin >> filename;
    fin.open(filename.c_str());

    scan();  // Read the first token, save as lookahead
    story(); // Call the <story> recursive descent function to start parsing
    
    fin.close(); // Close the input file
}
*/
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
