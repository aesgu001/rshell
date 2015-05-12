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

void print_error_token(const char *arg0, const std::string &s) {
    std::cout<<arg0<<": syntax error near unexpected token `"<<s<<"'"
        <<std::endl;
}

bool parse_help(std::queue<cmd> &commands, const std::string &l,
    const std::string &conn, const char *arg0) {
    if (is_blank(l)) {
        if (conn!="") {
            print_error_token(arg0,conn);
            return false;
        }
        return true;
    }
    char *c_l=new char[l.length()+1];
    strcpy(c_l,l.c_str());
    cmd command;
    command.set_exec(strtok(c_l," "));
    command.set_conn(conn);
    command.push_arg(arg0);
    char *p;
    std::string str_p,str_f;
    while (NULL!=(p=strtok(NULL," "))) {
        str_p=p;
        if (str_p==">"||str_p==">>") {
            command.set_ordir(str_p);
            if (NULL==(p=strtok(NULL," "))) {
                print_error_token(arg0,"newline");
                return false;
            }
            else {
                str_p=p;
                command.set_ofile(str_p);
            }
        }
        else if (str_p=="<") {
            if (NULL==(p=strtok(NULL," "))) {
                print_error_token(arg0,"newline");
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

void dump_queue(std::queue<cmd> &commands) {
    while (!commands.empty())
        commands.pop();
}

void parse(std::queue<cmd> &commands, const std::string &line,
    const char *arg0) {
    std::size_t pos_htag,pos_conn;
    if (std::string::npos!=(pos_htag=find_hashtag(line))) {
        parse(commands,line.substr(0,pos_htag),arg0);
        return;
    }
    else if (std::string::npos==(pos_conn=find_connector(line))) {
        if (!parse_help(commands,line,"",arg0))
            dump_queue(commands);
        return;
    }
    std::string conn=get_nearest_connector(line,pos_conn);
    if (!parse_help(commands,line.substr(0,pos_conn),conn,arg0))
        dump_queue(commands);
    else parse(commands,line.substr(pos_conn+conn.length(),std::string::npos),arg0);
}

#endif
