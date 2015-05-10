#ifndef EXECUTE_H
#define EXECUTE_H

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>
#include <vector>
#include "cmd.h"

bool execute_help(const cmd &command) {
    int status=0;
    std::vector<std::string> v=command.get_arlist();
    char **arlist=new char*[v.size()+1];
    for (std::size_t i=0;i<v.size();i++) {
        arlist[i]=new char[v.at(i).length()+1];
        strcpy(arlist[i],v.at(i).c_str());
    }
    arlist[v.size()]=NULL;
    pid_t pid=fork();
    if (pid<0) {
        perror("fork");
        exit(1);
    }
    else if (pid==0) {
        if (-1==execvp(command.get_executable().c_str(),arlist)) {
            perror("execvp");
            exit(1);
        }
        exit(0);
    }
    if (-1==waitpid(-1,&status,0)) {
        perror("wait");
        exit(1);
    }
    for (std::size_t i=0;i<v.size()+1;i++)
        delete[] arlist[i];
    delete[] arlist;
    return status==0? true:false;
}

void execute(std::queue<cmd> &commands, bool &exit_called) {
    cmd command;
    bool exec_success=false;
    while (!commands.empty()) {
        command=commands.front();
        commands.pop();
        if (command.get_executable()=="exit") {
            exit_called=true;
            return;
        }
        exec_success=execute_help(command);
        while (!commands.empty()&&
            ((exec_success&&command.get_connector()=="||")||
            (!exec_success&&command.get_connector()=="&&"))) {
            command.set_connector(commands.front().get_connector());
            commands.pop();
        }
    }
}

#endif
