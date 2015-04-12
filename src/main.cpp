#include <iostream>
#include <unistd.h>
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

void parse_help(const string &line, const string &conn,
    queue<cmd> &commands, char *s) {
    string exec=line.substr(0,line.find(" "));
    string l,p;
    cmd command;
    command.set_exec(exec);
    command.set_conn(conn);
    command.push(s);
    if (exec.length()!=line.length()) {
        l=line.substr(exec.length()+1,string::npos);
        p=l.substr(0,l.find(" "));
        while (p.length()!=l.length()) {
            command.push(p);
            l=l.substr(p.length()+1,string::npos);
            p=l.substr(0,l.find(" "));
        }
        command.push(p);
    }
    commands.push(command);
}

void parse(const string &line, queue<cmd> &commands, char *s) {
    string conn=get_nearest_connector(line);
    if (line=="") return;
    if (conn=="") { parse_help(line,conn,commands,s); return; }
    string l=line.substr(0,line.find(conn));
    parse_help(l,conn,commands,s);
    parse(line.substr(line.find(conn)+conn.size(),string::npos),commands,s);
}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    while (commands.empty()||commands.front().get_exec()!="exit") {
        cout << "$ ";
        getline(cin,line);
        parse(line,commands,argv[0]);
    }
    return 0;
}
