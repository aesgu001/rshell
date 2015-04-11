#include <iostream>
#include <unistd.h>
#include <queue>
#include "cmd.h"
using namespace std;

// also determines the number of commands to enqueue
void parse(string line) {}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    while (commands.empty()||commands.front().get_exec()!="exit") {
        cout << "$ ";
        cin >> line;
        parse(line);
    }
    return 0;
}
