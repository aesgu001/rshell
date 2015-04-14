#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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
    char *exec=strtok(c_line," ");
    cmd command;
    command.set_exec(exec);
    command.set_conn(conn);
    command.push(s);
    char *p=strtok(NULL," ");
    while (p!=0) {
        command.push(p);
        p=strtok(NULL," ");
    }
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
    int flag=0;
    int pid=fork();
    if (pid<0) {
        perror("fork");
    }
    else if (pid==0) {
        if (-1==execvp(command.get_exec(),command.get_arlist())) {
            perror("execvp");
            flag=-1;
        }
    }
    else // (pid>0)
        if (-1==wait(0))
            perror("wait");
    return (flag==-1)? false : true;
}

bool exit_command(char *c, const string &s) {
    char *c_s=new char[s.length()+1];
    strcpy(c_s,s.c_str());
    size_t i=0,j=0;
    for (char *p=c;*p!='\0';p++)
        i++;
    for (char *p=c_s;*p!='\0';p++)
        j++;
    if (i!=j) return false;
    for (size_t k=0;k<s.length();k++)
        if (*(c+k)!=*(c_s+k))
            return false;
    return true;
}

void execute(queue<cmd> &commands, bool &exit_called) {
    cmd command;
    bool exec_flag;
    while (!commands.empty()) {
        command=commands.front();
        commands.pop();
        if (exit_command(command.get_exec(),"exit")) {
            exit_called=true;
            return;
        }
        exec_flag=has_executed(command);
        if (((exec_flag&&command.get_conn()=="||")||(!exec_flag&&
            command.get_conn()=="&&"))&&
            !commands.empty()) commands.pop();
    }
}

int main(int argc, char **argv) {
    queue<cmd> commands;
    string line;
    bool exit_called=false;
    while (!exit_called) {
        cout << "$ ";
        getline(cin,line);
        parse(line,commands,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
