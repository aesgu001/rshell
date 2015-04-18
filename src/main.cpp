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
    if (-1==waitpid(-1,&status,0)) { perror("wait"); exit(1); }
    return status==0? true : false;
}

void execute(queue<cmd> &commands, bool &exit_called) {
    cmd command;
    bool exec_flag;
    while (!commands.empty()) {
        command=commands.front();
        commands.pop();
        if (command.get_exec()=="exit") { exit_called=true; return; }
        exec_flag=has_executed(command);
        while (((exec_flag&&command.get_conn()=="||")||(!exec_flag&&
            command.get_conn()=="&&"))&&
            !commands.empty()) {
            command.set_conn(commands.front().get_conn());
            commands.pop();
        }
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
        if (-1==gethostname(user_host,HOST_NAME_MAX)) {
            perror("gethostname"); exit(1);
        }
        cout << user_host << "$ ";
        getline(cin,line);
        parse(line,commands,argv[0]);
        execute(commands,exit_called);
    }
    return 0;
}
