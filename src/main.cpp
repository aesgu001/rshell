#include <iostream>
#include <queue>
#include "cmd.h"
#include "execute.h"
#include "login.h"
#include "parse.h"
#include "sig.h"
using namespace std;

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    bool exit_called=false;
    if (!sig_init(commands))
        return 1;
    while (!exit_called) {
        login(getenv("PWD"));
        getline(cin,line);
        parse(commands,line,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
