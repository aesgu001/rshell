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
        if (-1==execvp(command.get_exec(),arlist)) {
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
        if (strcmp(command.get_exec(),"exit")==0) {
            exit_called=true;
            return;
        }
        exec_success=execute_help(command);
        while (!commands.empty()&&
            ((exec_success&&strcmp(command.get_conn(),"||")==0)||
            (!exec_success&&strcmp(command.get_conn(),"&&")==0))) {
            command.set_conn(commands.front().get_conn());
            commands.pop();
        }
    }
}

#endif
