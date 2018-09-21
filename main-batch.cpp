#include <iostream>
#include "session.h"
#include "config.h"
#include "lineprocess.h"

using namespace std;

int main(int argc, char** argv) {
    bool running = true;
    string commandString;
    int commandsProcessed = 0;
    int commandsFailed = 0;

    Config::LoadConfig();
    if (!Config::IsConfigValid()) {
        cout << "Invalid config; check settings and restart." << endl;
        return -1;
    }

    Session::Init();

    while (running) {
        getline(cin, commandString);
        LineProcessExitStatus status = LineProcess::Process(commandString);
        switch (status) {
            case LineProcessExitStatus::LineProcessStatus_OK:
                ++commandsProcessed;
                break;
            case LineProcessExitStatus::LineProcessStatus_MISUNDERSTOOD:
            case LineProcessExitStatus::LineProcessStatus_FAIL:
                ++commandsProcessed;
                ++commandsFailed;
                break;
            case LineProcessExitStatus::LineProcessStatus_TERMINATE:
                running = false;
                break;
        }
    }
    cout << commandsProcessed - commandsFailed << " of " << commandsProcessed << " commands processed successfully." << endl;

    Session::Destroy();
    return 0;
}

