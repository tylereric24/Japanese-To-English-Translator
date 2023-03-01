#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include "scanner.h"
using namespace std;

/* Look for all **'s and complete them */

//=====================================================
// File scanner.cpp written by: Group Number: 18
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA 
// Done by: Nathan Hawrysh
// RE: (vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n)^+ 
bool word (string s)
{
    int state = 0;

    for (size_t charpos = 0; charpos < s.size(); charpos++) {
        if      (state == 0 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }
        else if (state == 0 && string("bghkmnpr").find(s[charpos]) != string::npos) { state = 1; }
        else if (state == 0 && string("dwzyj").find(s[charpos]) != string::npos) { state = 5; }
        else if (state == 0 && s[charpos] == 't') { state = 2; }
        else if (state == 0 && s[charpos] == 's') { state = 3; }
        else if (state == 0 && s[charpos] == 'c') { state = 4; }

        else if (state == 1 && s[charpos] == 'y') { state = 5; }
        else if (state == 1 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }

        else if (state == 2 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }
        else if (state == 2 && s[charpos] == 's') { state = 5; }

        else if (state == 3 && s[charpos] == 'h') { state = 5; }
        else if (state == 3 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }

        else if (state == 4 && s[charpos] == 'h') { state = 5; }

        else if (state == 5 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }

        else if (state == 6 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }
        else if (state == 6 && s[charpos] == 't') { state = 2; }
        else if (state == 6 && s[charpos] == 's') { state = 3; }
        else if (state == 6 && s[charpos] == 'c') { state = 4; }
        else if (state == 6 && string("dwzyj").find(s[charpos]) != string::npos) { state = 5; }
        else if (state == 6 && string("bghkmpr").find(s[charpos]) != string::npos) { state = 1; }
        else if (state == 6 && s[charpos] == 'n') { state = 7; }

        else if (state == 7 && string("EIaeiou").find(s[charpos]) != string::npos) { state = 6; }
        else if (state == 7 && s[charpos] == 't') { state = 2; }
        else if (state == 7 && s[charpos] == 's') { state = 3; }
        else if (state == 7 && s[charpos] == 'c') { state = 4; }
        else if (state == 7 && string("dwzyj").find(s[charpos]) != string::npos) { state = 5; }
        else if (state == 7 && string("bghkmnpr").find(s[charpos]) != string::npos) { state = 1; }

        else { return false; } // No matching transition
    }  
    return (state == 0 || state == 6 || state == 7);
}

// PERIOD DFA 
// Done by: Nathan Hawrysh
bool period (string s)
{
    int state = 0;

    for (size_t charpos = 0; charpos < s.size(); charpos++)
    {
        if (state == 0 && s[charpos] == '.') { state = 1; }
        else { return false; }
    }
    
    return state == 1;
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Ahmed Ali

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
//enum tokentype {ERROR, WORD1, WORD2, PERIOD, EOFM, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, BE};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {"ERROR", "WORD1", "WORD2", "PERIOD", "EOFM", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "BE"}; 

// ** Need the reservedwords table to be set up here. 
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.
string reservedWords[30] = {"masu", "masen", "mashita", "masendeshita", "desu", "deshita", "o", "wa", "ni", "watashi", "anata", "kare", "kanojo", "sore", "mata", "soshite", "shikashi", "dakara", "eofm"};

// Helper function created to easily compare a reserved word to its tokentype 
// Done: Ahmed Ali
tokentype reservedWordToken(const string& w)
{
  if (w == "masu")
    return VERB;
  else if (w == "masen")
    return VERBNEG;
  else if (w == "mashita")
    return VERBPAST;
  else if (w == "masendeshita")
    return VERBPASTNEG;
  else if (w == "desu")
    return IS;
  else if (w == "deshita")
    return WAS;
  else if (w == "o")
    return OBJECT;
  else if (w == "wa")
    return SUBJECT;
  else if (w == "ni")
    return DESTINATION;
  else if (w == "watashi" || w == "anata" || w == "kare" || w == "kanojo" || w == "sore")
    return PRONOUN;
  else if (w == "mata" || w == "soshite" || w == "shikashi" || w == "dakara")
    return CONNECTOR;
  else
    return EOFM;
}

// ------------ Scanner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

bool is_lower_vowel(const char thechar) {
    char vowels[5] = {'a', 'e', 'i', 'o', 'u'};
    for (unsigned int i = 0; i < 5; i++) {
        if (thechar == vowels[i]) { return true; }
    }
    return false;
}

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Ahmed Ali
void scanner(tokentype& tt, string& w)
{
    fin >> w; // Read the next word.

    // 1. If it is eofm, return right now.   
    if (w == "eofm")
    {
        tt = EOFM;
        return;
    }
    // 2. Call the token functions (word and period)  one after another (if-then-else).
    else if (word(w))
    {
        // 3. If it was a word, check against the reservedwords list.
        for (int i = 0; i < 30; i++)
        {
            if (w == reservedWords[i])
            {
                tt = reservedWordToken(w);
                return;
            }
        }
        
        // If not reserved, tokentype is WORD1 or WORD2 decided based on the last character.
        if (is_lower_vowel(w[w.size() - 1]) || // Ends in a vowel
            ( // Ends in a vowel + 'n'
                w[w.size() - 1] == 'n' && 
                is_lower_vowel(w[w.size() - 2])
            ))
        {
          tt = WORD1;
        }
        else if (w[w.size() - 1] == 'E' || w[w.size() - 1] == 'I')
        {
          tt = WORD2;
        }
        else { // This case shouldn't ever happen.
            cerr << "Lexical error: word " << w << " is not reserved, WORD1, nor WORD2" << endl;
            tt = ERROR; 
        }
          
    }
    else if (period(w))
    {
        tt = PERIOD;
    }
    // Generate a lexical error message if both DFAs failed. Let the tokentype be ERROR 
    //  in that case.
    else
    {
        cerr << "Lexical error: " << w << " is not a valid token." << endl;
        tt = ERROR; 
    }
}
  //the end of scanner

// The temporary test driver to just call the scanner repeatedly  
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!! 
// Done by:  Louis
/*
int main()
{
  tokentype thetype;
  string theword; 
  string filename;

  cout << "Enter the input file name: ";
  cin >> filename;

  fin.open(filename.c_str());
  
  // the loop continues until eofm is returned.
  while (true)
    {
       scanner(thetype, theword);  // call the scanner which sets
                                   // the arguments  
       if (theword == "eofm") break;  // stop now

       //cout << "Type is:" << tokenName[thetype] << endl;
       //cout << "Word is:" << theword << endl;   
       cout << theword << " is token type " << tokenName[thetype] << endl << endl;
    }

   cout << "End of file is encountered." << endl;
   fin.close();

}
*/