#include <iostream>
#include <queue>
#include "cmd.h"
#include "login.h"
#include "parse.h"
#include "execute.h"
using namespace std;

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    bool exit_called=false;
    while (!exit_called) {
        login();
        getline(cin,line);
        parse(commands,line,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
