#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Commands {

public:
    static bool CreateAccount(string email, string password, string realname, string linkedemail);
    static bool DeleteAccount(string email);
    static vector<string> GetEmailAccounts();
    static bool GetAccountInfo(string email, map<string, string>& info);
};
