#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
//#include <jsoncpp/json/json.h>
#include <json/json.h>

#include "config.h"

using namespace std;

string Config::PSQLInitString;
bool Config::ValidConfig = false;

bool Config::HasExistingConfig() {
    ifstream confFile(GetConfigPath());
    return confFile.good();
}

void Config::CreateTemplateConfig() {
    ofstream confFile(GetConfigPath());
    string confTemplate = "{\n"
                          "    \"dbname\":\"db\",\n"
                          "    \"user\":\"username\",\n"
                          "    \"password\":\"password\",\n"
                          "    \"hostaddr\":\"127.0.0.1\",\n"
                          "    \"port\":\"5432\"\n"
                          "}\n";
    confFile << confTemplate;
    confFile.close();
}

void Config::LoadConfig() {
    if (!Config::HasExistingConfig()) {
        Config::CreateTemplateConfig();
        cout << "Config file not found, created template at: " << GetConfigPath() << endl;
        cout << "Edit settings and re-run application." << endl;
        return;
    }

    cout << "Using configuration at: " << GetConfigPath() << endl;
    ifstream confFile(GetConfigPath());
    Json::Reader reader;
    Json::Value root;
    const Json::Value defValue;
    Json::Value field;
    bool invalid = false;
    stringstream ss;

    string dbname, user, password, hostaddr, port;

    bool jsonReaderResult = reader.parse(confFile,root);
    if (!jsonReaderResult) {
        cout << "Could not read config: " << reader.getFormattedErrorMessages();
    } else {
        field = root.get("dbname", defValue);
        if (field.isNull()) {
            cout << "dbname key does not exist in config!" << endl;
            invalid = true;
        } else {
            dbname = field.asString();
        }

        field = root.get("user", defValue);
        if (field.isNull()) {
            cout << "user key does not exist in config!" << endl;
            invalid = true;
        } else {
            user = field.asString();
        }

        field = root.get("password", defValue);
        if (field.isNull()) {
            cout << "password key does not exist in config!" << endl;
            invalid = true;
        } else {
            password = field.asString();
        }

        field = root.get("hostaddr", defValue);
        if (field.isNull()) {
            cout << "host key does not exist in config!" << endl;
            invalid = true;
        } else {
            hostaddr = field.asString();
        }

        field = root.get("port", defValue);
        if (field.isNull()) {
            cout << "port key does not exist in config!" << endl;
            invalid = true;
        } else {
            port = field.asString();
        }

        if (!invalid) {
            ss << "dbname = ";
            ss << dbname;
            ss << " user = ";
            ss << user;
            ss << " password = ";
            ss << password;
            ss << " hostaddr = ";
            ss << hostaddr;
            ss << " port = ";
            ss << port;

            PSQLInitString = ss.str();
            ValidConfig = true;
        }
    }

}

bool Config::IsConfigValid() {
    return ValidConfig;
}

string Config::GetConfigPath() {
    stringstream ss;
    ss << getenv("HOME") << "/.mail-admin.conf";
    return ss.str();
}

string Config::GetPSQLInitString() {
    return PSQLInitString;
}
