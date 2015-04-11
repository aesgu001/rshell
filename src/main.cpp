#include <iostream>
#include <unistd.h>
#include <queue>
#include "cmd.h"
using namespace std;

void parse(const string &line, queue<cmd> &commands, char *s) {

}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    parse(line,commands,argv[0]);
    return 0;
}
