#include <iostream>

#include "stringutil.h"

using namespace std;

void StringUtil::Tokenize(const string &str, const string &delim, vector<string> &tokens) {
    // out.clear();

    std::string token;
    std::string s(str);
    size_t pos = 0;

    while ((pos = s.find(delim)) != std::string::npos)
    {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delim.length());
    }

    tokens.push_back(s);
}
