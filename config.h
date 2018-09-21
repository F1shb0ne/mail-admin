#pragma once

#include <iostream>

using namespace std;

class Config {
private:
    static string GetConfigPath();
    static string PSQLInitString;
    static bool ValidConfig;

public:
    static bool HasExistingConfig();
    static void CreateTemplateConfig();
    static void LoadConfig();
    static bool IsConfigValid();
    static string GetPSQLInitString();
};

