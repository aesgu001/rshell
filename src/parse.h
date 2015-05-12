#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include <queue>
#include "cmd.h"

std::size_t find_hashtag(const std::string &s) {
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

std::size_t find_connector(const std::string &s) {
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
    char *c_l=new char[l.length()+1];
    strcpy(c_l,l.c_str());
    cmd command;
    command.set_exec(strtok(c_l," "));
    command.set_conn(conn);
    command.push_arg(s);
    char *p;
    std::string str_p;
    p=strtok(NULL," ");
    while (p!=NULL) {
        str_p=p;
        command.push_arg(str_p);
        p=strtok(NULL," ");
    }
    commands.push(command);
    delete[] c_l;
    return true;
}

void dump_queue(std::queue<cmd> &commands) {
    while (!commands.empty())
        commands.pop();
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
