//
// Created by elvis on 11/8/17.
//

#ifndef PL0_LEX_ANALYSIS_H
#define PL0_LEX_ANALYSIS_H

#include <cstdio>
#include <iostream>
#include <fstream>

#include <string>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <map>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <boost/assert.hpp>

using namespace std;

char lex_character;

string lex_segmentation_sentence;
ifstream lex_in;

/*
void read_file(string file_name)
{
    in.open(file_name.data());
    assert(in.is_open());
    in >> noskipws;
}

bool is_file_end()
{
    return !in.eof();
}

char read_character()
{
    in >> character;
    return character;
}

void close_file()
{
    in.close();
}
*/

#endif //PL0_LEX_ANALYSIS_H