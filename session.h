#pragma once

#include <pqxx/pqxx>

class Session {
private:
    static pqxx::connection* Connection;
    static bool Valid;
public:
    static void Init();
    static void Destroy();
    static bool IsValid();
    static pqxx::connection* GetConnection();
};
