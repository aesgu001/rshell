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

void parse_help(const string &line, const string &conn, queue<cmd> commands,
    char *s) {
    char *c_line=new char[line.length()+1];
    strcpy(c_line,line.c_str());
    string exec=strtok(c_line," ");
    char *p=strtok(NULL," ");
    while (p!=0) { p=strtok(NULL," "); }
    //commands.push(cmd(exec,arlist,conn));
    delete[] c_line;
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
    while (true) {
        getline(cin,line);
        parse(line,commands,argv[0]);
        //while (!commands.empty()) {
        //    commands.front().print();
        //    commands.pop();
        //}
    }
    return 0;
}
