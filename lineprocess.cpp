#include <iostream>
#include <vector>
#include <map>
#include <regex>

#include "stringutil.h"
#include "lineprocess.h"
#include "commands.h"
#include "crypto.h"

using namespace std;

LineProcessExitStatus LineProcess::Process(string commandString) {
    vector<string> commandTokens;
    string delimiter = " ";
    StringUtil::Tokenize(commandString, delimiter, commandTokens);
    int numTokens = (int)commandTokens.size();

    if (commandTokens[0] == "accountlist") {
        vector<string> accountList = Commands::GetEmailAccounts();
        for (string account : accountList) {
            cout << account << endl;
        }
        return LineProcessExitStatus::LineProcessStatus_OK;
    }

    if (commandTokens[0] == "accountdelete") {
        if (numTokens != 2) {
            cout << "Usage: accountdelete emailaddress@domain.com" << endl;
            return LineProcessExitStatus::LineProcessStatus_MISUNDERSTOOD;
        }
        if (Commands::DeleteAccount(commandTokens[1])) {
            cout << commandTokens[1] << " removed." << endl;
            return LineProcessExitStatus::LineProcessStatus_OK;
        } else {
            cout << "Could not remove " << commandTokens[1] << endl;
            return LineProcessExitStatus::LineProcessStatus_FAIL;
        }
    }

    // check if a password term is provided
    if (commandTokens[0] == "accountadd") {
        if (numTokens < 5) {
            cout << "Usage: accountadd email=\"address\" password=\"password\" realname=\"name\" linkedemail=\"address\"" << endl;
            return LineProcessExitStatus::LineProcessStatus_MISUNDERSTOOD;
        }
        map<string, string> newAccountTerms;
        newAccountTerms["email"] = GetQuotedTokenInString(commandString, "email");
        newAccountTerms["password"] = GetQuotedTokenInString(commandString, "password");
        newAccountTerms["realname"] = GetQuotedTokenInString(commandString, "realname");
        newAccountTerms["linkedemail"] = GetQuotedTokenInString(commandString, "linkedemail");

        // make sure all string lengths are >0 (hopefully valid)
        bool meetsLengthReq = true;
        for (map<string, string>::iterator it = newAccountTerms.begin(); it != newAccountTerms.end(); ++it) {
            if (it->second.length() == 0) {
                meetsLengthReq = false;
                cout << "Term " << it->first << " is empty!" << endl;
            }
        }

        if (meetsLengthReq) {
            bool addaccount_result = Commands::CreateAccount(
                    newAccountTerms["email"],
                    CryptoHash(newAccountTerms["password"]).GetDigest(),
                    newAccountTerms["realname"],
                    newAccountTerms["linkedemail"]);
            if (addaccount_result) {
                cout << newAccountTerms["email"] << " added successfully." << endl;
                return LineProcessExitStatus::LineProcessStatus_OK;
            } else {
                cout << newAccountTerms["email"] << " could NOT be added." << endl;
                return LineProcessExitStatus::LineProcessStatus_FAIL;
            }
        } else {
            cout << "accountadd action DID NOT meet requirements!" << endl;
            return LineProcessExitStatus::LineProcessStatus_FAIL;
        }
    }

    if (commandTokens[0] == "quit") {
        return LineProcessExitStatus::LineProcessStatus_TERMINATE;
    }

    // If we get here the command was not understood
    cout << "Command not understood" << endl;

    return LineProcessExitStatus::LineProcessStatus_MISUNDERSTOOD;
}

string LineProcess::GetQuotedTokenInString(string line, string token) {
    stringstream ss;
    ss << token;
    ss << "\\s*=\\s*\"([^\"]+)\"";
    regex regex_quoted_token(ss.str());
    smatch match;
    string result = "";

    if (regex_search(line, match, regex_quoted_token)) {
        result = match.str(1);
    }

    return result;
}
