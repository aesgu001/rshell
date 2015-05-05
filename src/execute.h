#ifndef EXECUTE_H
#define EXECUTE_H

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>
#include "cmd.h"

bool execute_help(const cmd &command) {
    int status=0;
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
        exit(0);
    }
    if (-1==waitpid(-1,&status,0)) {
        perror("wait");
        exit(1);
    }
    return status==0? true:false;
}

void execute(std::queue<cmd> &commands, bool &exit_called) {
    cmd command;
    bool exec_success=false;
    while (!commands.empty()) {
        command=commands.front();
        commands.pop();
        if (command.get_exec()=="exit") {
            exit_called=true;
            return;
        }
        exec_success=execute_help(command);
        while (!commands.empty()&&
            ((exec_success&&command.get_conn()=="||")||
            (!exec_success&&command.get_conn()=="&&"))) {
            command.set_conn(commands.front().get_conn());
            commands.pop();
        }
    }
}

#endif
