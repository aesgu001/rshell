#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include <queue>
#include "cmd.h"

bool parse_isblank(const std::string &s) {
    for (std::size_t i=0;i<s.length();i++)
        if (s.at(i)!=' ')
            return false;
    return true;
}

void parse_perrtok(const char *arg0, const char *tok) {
    std::cout<<arg0<<": syntax error near unexpected token `"<<tok<<"'"
        <<std::endl;
}

bool parse_help(std::queue<cmd> &commands, const std::string &l,
    const std::string &conn, const char *arg0) {
    if (parse_isblank(l)) {
        if (conn!="") {
            parse_perrtok(arg0,conn.c_str());
            return false;
        }
        return true;
    }
    cmd command;
    char *c_l=new char[l.length()+1],*p;
    std::string str_p;
    strcpy(c_l,l.c_str());
    command.set_exec(strtok(c_l," "));
    command.push_arg(arg0);
    command.set_conn(conn);
    while (NULL!=(p=strtok(NULL," "))) {
        str_p=p;
        if (str_p==">"||str_p==">>") {
            command.set_ordir(str_p);
            if (NULL==(p=strtok(NULL," "))) {
                parse_perrtok(arg0,"newline");
                delete[] c_l;
                return false;
            }
            else {
                str_p=p;
                command.set_ofile(str_p);
            }
        }
        else if (str_p=="<") {
            if (NULL==(p=strtok(NULL," "))) {
                parse_perrtok(arg0,"newline");
                delete[] c_l;
                return false;
            }
            else {
                str_p=p;
                command.set_ifile(str_p);
            }
        }
        else command.push_arg(str_p);
    }
    commands.push(command);
    delete[] c_l;
    return true;
}

std::size_t parse_findhtag(const std::string &s) {
    for (std::size_t i=0;i<s.length();i++) {
        if (s.at(i)=='"') {
            for (std::size_t j=i+1;j<s.length();j++)
                if (s.at(j)=='"') {
                    i=j;
                    break;
                }
            continue;
        }
        else if (s.at(i)=='#') {
            return i;
        }
    }
    return std::string::npos;
}

std::size_t parse_findconn(const std::string &s) {
    for (std::size_t i=0;i<s.length();i++) {
        if (s.at(i)=='"') {
            for (std::size_t j=i+1;j<s.length();j++)
                if (s.at(j)=='"') {
                    i=j;
                    break;
                }
            continue;
        }
        else if (s.at(i)==';') {
            return i;
        }
        else if (s.at(i)=='&') {
            if (i+1<s.length()&&s.at(i+1)=='&')
                return i;
        }
        else if (s.at(i)=='|')
                return i;
    }
    return std::string::npos;
}

std::string parse_getconn(const std::string &s,
    const std::size_t &pos_conn) {
    if (pos_conn==std::string::npos) return "";
    else if (s.at(pos_conn)=='|') {
        if (pos_conn+1<s.length()&&s.at(pos_conn+1)=='|')
            return "||";
        return "|";
    }
    else if (s.at(pos_conn)=='&') return "&&";
    return ";";
}

void parse_dumpqueue(std::queue<cmd> &commands) {
    while (!commands.empty())
        commands.pop();
}

void parse(std::queue<cmd> &commands, const std::string &line,
    const char *arg0) {
    std::size_t pos_htag,pos_conn;
    if (std::string::npos!=(pos_htag=parse_findhtag(line))) {
        parse(commands,line.substr(0,pos_htag),arg0);
        return;
    }
    else if (std::string::npos==(pos_conn=parse_findconn(line))) {
        if (!parse_help(commands,line,"",arg0))
            parse_dumpqueue(commands);
        return;
    }
    std::string conn=parse_getconn(line,pos_conn);
    if (!parse_help(commands,line.substr(0,pos_conn),conn,arg0))
        parse_dumpqueue(commands);
    else parse(commands,line.substr(pos_conn+conn.length(),std::string::npos),
        arg0);
}

#endif
