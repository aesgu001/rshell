#include <iostream>
#include <unistd.h>
#include <string.h>
#include <queue>
#include "cmd.h"
using namespace std;

int nearest_connector(const string &s) {
    if (s.find(";")==string::npos && s.find("&&")==string::npos &&
        s.find("||")==string::npos) return -1;
    return min(min(s.find("&&"),s.find(";")),min(s.find(";"),s.find("||")));
}

string get_nearest_connector(const string &s) {
    int pos=nearest_connector(s);
    if (pos==-1) return "";
    if (s.at(pos)=='|') return "||";
    else if (s.at(pos)=='&') return "&&";
    return ";";
}

void parse(const string &line, queue<cmd> &commands) {
    string conn=get_nearest_connector(line);
    if (conn=="") { return; }
    string l=line.substr(0,line.find(conn));
    parse(line.substr(line.find(conn)+conn.size(),string::npos),commands);
}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    getline(cin,line);
    parse(line,commands);
    return 0;
}
