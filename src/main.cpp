#include <iostream>
#include <queue>
#include <signal.h>
#include "cmd.h"
#include "login.h"
#include "sig.h"
#include "parse.h"
#include "execute.h"
using namespace std;

int main(int argc, char **argv) {
    struct sigaction act;
    queue<cmd> commands;
    string line;
    bool exit_called=false;
    act.sa_handler=handler;
    act.sa_flags=SA_RESTART;
    sigemptyset(&act.sa_mask);
    if (!sig_init(SIGINT,act))
        return 1;
    while (!exit_called) {
        login();
        getline(cin,line);
        parse(commands,line,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
