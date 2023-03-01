#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>

// -----  Tables -------------------------------------

enum tokentype {ERROR, WORD1, WORD2, PERIOD, EOFM, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, BE};

// ------------ Scanner and Driver -----------------------

void scanner(tokentype& a, std::string& w);