#include <iostream>
#include <string.h>
#include <queue>
#include "cmd.h"
#include "login.h"
#include "execute.h"
using namespace std;

void skip_quote(const string &s, size_t &pos) {

    for (size_t i=pos+1;i<s.length();i++)
        if (s.at(i)=='"') {
            pos=i;
            return;
        }
}

size_t nearest_hashtag(const string &s) {
    for (size_t i=0;i<s.length();i++) {
        if (s.at(i)=='"') {
            skip_quote(s,i);
            continue;
        }
        else if (s.at(i)=='#') {
            return i;
        }
    }
    return string::npos;
}

size_t nearest_connector(const string &s) {
    for (size_t i=0;i<s.length();i++) {
        if (s.at(i)=='"') {
            skip_quote(s,i);
            continue;
        }
        else if (s.at(i)==';') {
            return i;
        }
        else if (s.at(i)=='&') {
            if (i+1<s.length()&&s.at(i+1)=='&')
                return i;
        }
        else if (s.at(i)=='|') {
            if (i+1<s.length()&&s.at(i+1)=='|')
                return i;
        }
    }
    return string::npos;
}

string get_nearest_connector(const string &s, const size_t &pos) {
    if (pos==string::npos) return "";
    else if (s.at(pos)=='|') return "||";
    else if (s.at(pos)=='&') return "&&";
    else return ";";
}

bool is_blank(const string &s) {
    for (size_t i=0;i<s.length();i++)
        if (s.at(i)!=' ')
            return false;
    return true;
}

void parse_help(const string &line, const string &conn,
    queue<cmd> &commands, char *s) {
    if (is_blank(line)) return;
    char *c_line=new char[line.length()+1];
    strcpy(c_line,line.c_str());
    string exec=strtok(c_line," ");
    cmd command;
    command.set_exec(exec);
    command.set_conn(conn);
    command.push(s);
    char *p=strtok(NULL," ");
    while (p!=0) {
        command.push(p);
        p=strtok(NULL," ");
    }
    command.push(NULL);
    commands.push(command);
}

void parse(const string &line, queue<cmd> &commands, char *s) {
    size_t pos_ht=nearest_hashtag(line);
    if (pos_ht!=string::npos) {
        parse(line.substr(0,pos_ht),commands,s);
        return;
    }
    size_t pos=nearest_connector(line);
    if (pos==string::npos) {
        parse_help(line,"",commands,s);
        return;
    }
    string conn=get_nearest_connector(line,pos);
    string l=line.substr(0,pos);
    parse_help(l,conn,commands,s);
    parse(line.substr(pos+conn.size(),string::npos),commands,s);
}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    bool exit_called=false;
    while (!exit_called) {
        login();
        getline(cin,line);
        parse(line,commands,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
