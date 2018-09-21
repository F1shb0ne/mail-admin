#pragma once

#include <iostream>

using namespace std;

class CryptoHash {
private:
    string MessageDigest;
    bool Valid;

public:
    CryptoHash(string input);
    string GetDigest();
    bool isValid();
};
