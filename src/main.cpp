#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
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

bool is_blank(const string &s) {
    for (size_t i=0;i<s.length();i++)
        if (s.at(i)!=' ')
            return false;
    return true;
}

void parse_help(const string &line, const string &conn,
    queue<cmd> &commands, char *s) {
    if (line==""||is_blank(line)) return;
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
    if (line.find("#")!=string::npos) {
        parse(line.substr(0,line.find("#")),commands,s);
        return;
    }
    string conn=get_nearest_connector(line);
    if (conn=="") { parse_help(line,conn,commands,s); return; }
    string l=line.substr(0,line.find(conn));
    parse_help(l,conn,commands,s);
    parse(line.substr(line.find(conn)+conn.size(),string::npos),commands,s);
}

bool has_executed(const cmd &command) {
    pid_t pid=fork();
    if (pid<0) {
        perror("fork");
        exit(1);
    }
    else if (pid==0) {
        if (-1==execvp(command.get_exec().c_str(),command.get_arlist())) {
            perror("execvp");
            exit(1);
        }
        else exit(0);
    }
    int status=0;
    if (-1==wait(&status)) { perror("wait"); exit(1); }
    int c_ret=WEXITSTATUS(status);
    return c_ret==0? true : false;
}

void execute(queue<cmd> &commands, bool &exit_called) {
    cmd command;
    bool exec_flag;
    while (!commands.empty()) {
        command=commands.front();
        commands.pop();
        if (command.get_exec()=="exit") { exit_called=true; return; }
        exec_flag=has_executed(command);
        if (((exec_flag&&command.get_conn()=="||")||(!exec_flag&&
            command.get_conn()=="&&"))&&
            !commands.empty()) commands.pop();
    }
}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    char *user_host;
    bool exit_called=false;
    while (!exit_called) {
        user_host=getlogin();
        if (user_host==NULL) { perror("getlogin"); exit(1); }
        cout << user_host << "@";
        if (-1==gethostname(user_host,HOST_NAME_MAX)) { perror("hostname"); exit(1); }
        cout << user_host << "$ ";
        getline(cin,line);
        parse(line,commands,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
