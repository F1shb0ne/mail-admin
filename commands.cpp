#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <pqxx/pqxx>

#include "session.h"
#include "commands.h"

using namespace std;

bool Commands::CreateAccount(string email, string password, string realname, string linkedemail) {
    string mboxname = "Unknown";
    regex regex_mboxname("(.*)@.*");
    smatch match;
    stringstream ss;

    if (regex_search(email, match, regex_mboxname)) {
        ss << match.str(1);
        ss << "/";
        mboxname = ss.str();
    } else {
        cout << "Could not get mboxname" << endl;
        return false;
    }

    ss.str("");
    ss << "insert into users (email, password, realname, maildir, linkedemail) values ('";
    ss << email;
    ss << "', '";
    ss << password;
    ss << "', '";
    ss << realname;
    ss << "', '";
    ss << mboxname;
    ss << "', '";
    ss << linkedemail;
    ss << "')";
    string sql = ss.str();

    if (Session::IsValid()) {
        try {
            pqxx::connection* conn = Session::GetConnection();
            pqxx::work transaction(*conn);
            transaction.exec(sql);
            transaction.commit();
        } catch (const exception& e) {
            cout << "Exception caught:" << endl;
            cout << e.what() << endl;
            return false;
        }
        return true;
    } else {
        cout << "Invalid configuration" << endl;
    }

    return false;
}

// Return true upon successful removal
bool Commands::DeleteAccount(string email) {
    stringstream ss;
    pqxx::result r;

    ss << "delete from users where email = '";
    ss << email;
    ss << "'";
    string sql = ss.str();

    if (Session::IsValid()) {
        try {
            pqxx::connection* conn = Session::GetConnection();
            pqxx::work transaction(*conn);
            r = transaction.exec(sql);
            transaction.commit();
        } catch (const exception& e) {
            cout << "Exception caught:" << endl;
            cout << e.what() << endl;
            return false;
        }

        if (r.affected_rows() == 0) {
            return false;
        }
        return true;
    } else {
        cout << "Invalid configuration" << endl;
    }

    return false;
}

vector<string> Commands::GetEmailAccounts() {
    vector<string> accounts;
    string sql_select = "select * from users";

    if (Session::IsValid()) {
        try {
            pqxx::connection* conn = Session::GetConnection();
            pqxx::nontransaction N(*conn);

            pqxx::result result;
            result = N.exec(sql_select);

            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c) {
                accounts.push_back(c[0].as<string>());
            }

        } catch (const exception &e) {
            cout << e.what() << endl;
        }
    } else {
        cout << "Invalid configuration" << endl;
    }

    return accounts;
}

bool Commands::GetAccountInfo(string email, map<string, string>& info) {
    stringstream ss;
    //(email, password, realname, maildir, linkedemail)
    ss << "select realname, password, maildir, linkedemail from users where email ='";
    ss << email;
    ss << "'";
    string sql_query = ss.str();

    if (Session::IsValid()) {
        try {
            pqxx::connection* conn = Session::GetConnection();
            pqxx::nontransaction ntrans(*conn);

            pqxx::result result;
            result = ntrans.exec(sql_query);
            if (result.size() == 1) {
                // Can only be one record with email address as primary key.
                pqxx::result::const_iterator record = result.begin();
                info["realname"] = record[0].as<string>();
                info["passwordhash"] = record[1].as<string>();
                info["maildir"] = record[2].as<string>();
                info["linkedemail"] = record[3].as<string>();
                return true;
            } else if (result.size() > 1) {
                cout << "Multiple accounts found! wtf?" << endl;
                return false;
            } else {
                cout << "Account not found." << endl;
                return false;
            }
        } catch (const exception &e) {
            cout << e.what() << endl;
        }
    } else {
        cout << "Invalid configuration" << endl;
    }
    return false;
}
