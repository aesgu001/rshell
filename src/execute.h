#ifndef EXECUTE_H
#define EXECUTE_H

#include <fcntl.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include "cd.h"
#include "cmd.h"

bool waiting=false;

bool execute_redirect(const bool &flag_irdir, const bool &flag_ordir,
    const int &fdi, const int &fdo) {
    if (flag_irdir)
        if (-1==dup2(fdi,0)) {
            perror("dup2");
            return false;
        }
    if (flag_ordir)
        if (-1==dup2(fdo,1)) {
            perror("dup2");
            return false;
        }
    return true;
}

bool execute_closefd(const bool &flag_irdir, const bool &flag_ordir,
    int &fdi, int &fdo) {
    if (flag_irdir) {
        if (-1==close(fdi)) {
            perror("close");
            return false;
        }
    }
    if (flag_ordir) {
        if (-1==close(fdo)) {
            perror("close");
            return false;
        }
    }
    return true;
}

bool execute_help(const cmd &command) {
    if (strcmp(command.get_exec(),"cd")==0)
        return cd(command);
    char **arlist=new char*[command.get_arlist().size()+1];
    bool flag_irdir=false,flag_ordir=false;
    int status=0,fdi,fdo;
    pid_t pid;
    for (std::size_t i=0;i<command.get_arlist().size();i++) {
        arlist[i]=new char[command.get_arlist().at(i).length()+1];
        strcpy(arlist[i],command.get_arlist().at(i).c_str());
    }
    arlist[command.get_arlist().size()]=NULL;
    if (strcmp(command.get_ifile(),"")!=0) {
        flag_irdir=true;
        if (-1==(fdi=open(command.get_ifile(),O_RDONLY))) {
            perror("open");
            for (std::size_t i=0;i<command.get_arlist().size()+1;i++)
                delete[] arlist[i];
            delete[] arlist;
            return false;
        }
    }
    if (strcmp(command.get_ofile(),"")!=0) {
        flag_ordir=true;
        int flags=command.get_ordir()==">"? O_WRONLY|O_CREAT|O_TRUNC:
            O_WRONLY|O_CREAT|O_APPEND;
        if (-1==(fdo=open(command.get_ofile(),flags,S_IRUSR|S_IWUSR))) {
            perror("open");
            exit(1);
        }
    }
    if (0>(pid=fork())) {
        perror("fork");
        exit(1);
    }
    else if (pid==0) {
        if (!execute_redirect(flag_irdir,flag_ordir,fdi,fdo))
            exit(1);
        if (-1==execvp(command.get_exec(),arlist)) {
            perror("execvp");
            exit(1);
        }
    }
    waiting=true;
    if (-1==waitpid(-1,&status,0)) {
        perror("waitpid");
        exit(1);
    }
    waiting=false;
    if (!execute_closefd(flag_irdir,flag_ordir,fdi,fdo))
        exit(1);
    for (std::size_t i=0;i<command.get_arlist().size()+1;i++)
        delete[] arlist[i];
    delete[] arlist;
    return status==0? true:false;
}

bool execute_savefd(int &sstdin, int &sstdout) {
    if (-1==(sstdin=dup(0))) {
        perror("dup");
        return false;
    }
    if (-1==(sstdout=dup(1))) {
        perror("dup");
        return false;
    }
    return true;
}

bool execute_restorefd(int &fd, const int &sstdin, const int &sstdout) {
    if (-1==close(fd)) {
        perror("close");
        return false;
    }
    if (-1==dup2(sstdin,0)) {
        perror("dup2");
        return false;
    }
    if (-1==dup2(sstdout,1)) {
        perror("dup2");
        return false;
    }
    return true;
}

bool execute_pipe(std::queue<cmd> &commands, cmd &command) {
    if (commands.empty()) {
        std::cout<<command.get_arlist().at(0)<<": missing operand for token `|'"
            <<std::endl;
        return false;
    }
    int pipefd[2],sstdin,sstdout;
    pid_t pid;
    if (!execute_savefd(sstdin,sstdout))
        exit(1);
    if (-1==pipe(pipefd)) {
        perror("pipe");
        exit(1);
    }
    if (0>(pid=fork())) {
        perror("fork");
        exit(1);
    }
    else if (pid==0) {
        if (-1==close(pipefd[0])) {
            perror("close");
            exit(1);
        }
        if (-1==dup2(pipefd[1],1)) {
            perror("dup2");
            exit(1);
        }
        if (!execute_help(command))
            exit(1);
        exit(0);
    }
    if (-1==close(pipefd[1])) {
        perror("close");
        exit(1);
    }
    if (-1==dup2(pipefd[0],0)) {
        perror("dup2");
        exit(1);
    }
    if (-1==wait(NULL)) {
        perror("wait");
        exit(1);
    }
    command=commands.front();
    commands.pop();
    if (command.get_conn()=="|") {
        if (!execute_pipe(commands,command)) {
            if (!execute_restorefd(pipefd[0],sstdin,sstdout))
                exit(1);
            return false;
        }
    }
    else if (!execute_help(command)) {
        if (!execute_restorefd(pipefd[0],sstdin,sstdout))
            exit(1);
        return false;
    }
    if (!execute_restorefd(pipefd[0],sstdin,sstdout))
        exit(1);
    return true;
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
        if (command.get_conn()=="|")
            exec_success=execute_pipe(commands,command);
        else exec_success=execute_help(command);
        while (!commands.empty()&&
            ((exec_success&&command.get_conn()=="||")||
            (!exec_success&&command.get_conn()=="&&"))) {
            command.set_conn(commands.front().get_conn());
            commands.pop();
        }
    }
}

#endif
