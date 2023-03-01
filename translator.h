#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include "scanner.h"

std::string saved_J_word;
std::string saved_E_word;
tokentype saved_lexme = ERROR;

// forward-declare non-terminal and translation functions
void story();
void s();
void noun();
void after_subject();
void verb();
void tense();
void after_noun();
void be();
void after_object();

void getEword();
void gen(std::string line_type);