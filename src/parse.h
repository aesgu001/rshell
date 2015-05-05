#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include <queue>
#include "cmd.h"

void skip_quote(const std::string &s, std::size_t &pos) {
    for (std::size_t i=pos+1;i<s.length();i++)
        if (s.at(i)=='"') {
            pos=i;
            return;
        }
}

std::size_t find_hashtag(const std::string &s) {
    for (size_t i=0;i<s.length();i++) {
        if (s.at(i)=='"') {
            skip_quote(s,i);
            continue;
        }
        else if (s.at(i)=='#') {
            return i;
        }
    }
    return std::string::npos;
}

std::size_t find_connector(const std::string &s) {
    for (std::size_t i=0;i<s.length();i++) {
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
    return std::string::npos;
}

std::string get_nearest_connector(const std::string &s,
    const std::size_t &pos_conn) {
    if (pos_conn==std::string::npos) return "";
    else if (s.at(pos_conn)=='|') return "||";
    else if (s.at(pos_conn)=='&') return "&&";
    return ";";
}

bool is_blank(const std::string &s) {
    for (std::size_t i=0;i<s.length();i++)
        if (s.at(i)!=' ')
            return false;
    return true;
}

void dump_queue(std::queue<cmd> &commands) {
    while (!commands.empty())
        commands.pop();
}

bool parse_help(std::queue<cmd> &commands, const std::string &l,
    const std::string &conn, char *s) {
    if (is_blank(l)) {
        if (conn!="") {
            std::cout<<s<<": syntax error near unexpected token `"<<conn<<"'"
                <<std::endl;
            return false;
        }
        return true;
    }
    cmd command;
    std::string exec;
    char *c_line=new char[l.length()+1],*p;
    strcpy(c_line,l.c_str());
    exec=strtok(c_line," ");
    command.set_exec(exec);
    command.set_conn(conn);
    command.push(s);
    p=strtok(NULL," ");
    while (p!=0) {
        command.push(p);
        p=strtok(NULL," ");
    }
    command.push(NULL);
    commands.push(command);
    return true;
}

void parse(std::queue<cmd> &commands, const std::string &line, char *s) {
    std::size_t pos_htag,pos_conn;
    if (std::string::npos!=(pos_htag=find_hashtag(line))) {
        parse(commands,line.substr(0,pos_htag),s);
        return;
    }
    else if (std::string::npos==(pos_conn=find_connector(line))) {
        if (!parse_help(commands,line,"",s))
            dump_queue(commands);
        return;
    }
    std::string conn=get_nearest_connector(line,pos_conn);
    if (!parse_help(commands,line.substr(0,pos_conn),conn,s))
        dump_queue(commands);
    else parse(commands,line.substr(pos_conn+conn.length(),std::string::npos),s);
}

#endif
