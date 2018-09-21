#pragma once

#include <iostream>

using namespace std;

typedef enum {
    LineProcessStatus_OK,
    LineProcessStatus_MISUNDERSTOOD,
    LineProcessStatus_FAIL,
    LineProcessStatus_TERMINATE
} LineProcessExitStatus;

class LineProcess {
public:
    static LineProcessExitStatus Process(string line);
    static string GetQuotedTokenInString(string line, string token);
};
