#include <iostream>
#include "session.h"
#include "config.h"

using namespace std;

pqxx::connection* Session::Connection = nullptr;
bool Session::Valid = false;

bool Session::IsValid() {
    return Valid;
}

pqxx::connection *Session::GetConnection() {
    return Connection;
}

void Session::Init() {
    try {
        Connection = new pqxx::connection(Config::GetPSQLInitString());
        if (Connection->is_open()) {
            Valid = true;
        } else {
            cout << "Something fucked up." << endl;
        }

    } catch (const exception& e) {
        cout << e.what() << endl;
    }
}

void Session::Destroy() {
    if (Valid) {
        Connection->disconnect();
        Valid = false;
    }
}

