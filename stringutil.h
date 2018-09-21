#pragma once

#include <iostream>
#include <vector>

using namespace std;

class StringUtil {

public:
    static void Tokenize(const string &str, const string &delim, vector<string> &tokens);

};
