#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "translator.h"
#include "parser.cpp"

using namespace std;


//=================================================
// File translator.cpp written by Group Number: 18
//=================================================

// ----- Additions to the parser.cpp ---------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt 
//  Done by: Nathan Hawrysh
unordered_map<string, string> lexicon;

ofstream translated = ofstream();

// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result   
//                 in saved_E_word
//  Done by: Ahmed Ali

void getEword() {
    unordered_map<string, string>::const_iterator found = lexicon.find(saved_J_word);
    saved_E_word = (found != lexicon.end())
        ? found->second
        : saved_J_word;
}

//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
//  Done by: Ahmed Ali

void gen(string line_type) {
    if (line_type == "TENSE") {
        translated << "TENSE: " << tokenName[saved_lexme] << endl << endl;
    }
    else {
        translated << line_type << ": " << saved_E_word << endl;
    }
}

// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar 
//    rule with semantic routine calls
//    above each non-terminal function 

// ** Each non-terminal function should be calling
//    getEword and/or gen now.


// ---------------- Driver ---------------------------

/** Opens the lexicon.txt file and reads it into `lexicon`.
  * Done by: Nathan Hawrysh */

template<class T>
T open_file(const string& filename) {
    T file;
    file.open(filename);
    if (file.fail()) {
        perror(("Could not open " + filename).c_str());
        exit(1);
    }
    return file;
}

/** Opens the file lexicon.txt and reads it into `lexicon`.
  * Done by: Nathan Hawrysh */
void populate_lexicon() {
    ifstream lex_file = open_file<ifstream>("./lexicon.txt");
    string jp, en;
    while (lex_file >> jp) {
        lex_file >> en;
        lexicon.insert(make_pair(jp, en));
    }
    // `close` is automatically called by destructor
}

// The final test driver to start the translator
// Done by: Nathan Hawrysh
int main()
{
    populate_lexicon(); // opens the input file translated.txt
    translated.open("./translated.txt"); // opens the output file translated.txt
    
    cout << "Enter the input file name: ";
    cin >> filename;
    fin.open(filename.c_str());

    scan();  // Read the first token, save as lookahead
    story(); // calls the <story> to start parsing

    // `close` is automatically called by file stream destructors
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
