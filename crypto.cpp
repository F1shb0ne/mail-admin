#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <sstream>
#include "crypto.h"

using namespace std;

CryptoHash::CryptoHash(string input) {
    char md[64];
    char mds[129];
    char digits[] = "0123456789ABCDEF";
    MessageDigest = "INVALID";

    stringstream ss;
    string MessageDigestString;

    Valid = false;

    // Init hash context
    SHA512_CTX context;
    if (!SHA512_Init(&context)) {
        return;
    }

    // Load in the data
    if (!SHA512_Update(&context, input.c_str(), input.length())) {
        return;
    }

    // Finalize / compute hash
    if (!SHA512_Final((unsigned char*)md, &context)) {
        return;
    }

    // Convert binary hash to string
    for (int i = 0; i < 64; ++i) {
        mds[i*2] = digits[(unsigned char)md[i] >> 4];
        mds[i*2+1] = digits[((unsigned char)md[i]) & 0xf];
    }
    mds[128] = 0;

    MessageDigest = string(mds);
    Valid = true;
}

string CryptoHash::GetDigest() {
    return MessageDigest;
}

bool CryptoHash::isValid() {
    return Valid;
}
