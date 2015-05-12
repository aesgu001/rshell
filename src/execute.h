#ifndef EXECUTE_H
#define EXECUTE_H

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>
#include <unistd.h>
#include <vector>
#include "cmd.h"

bool execute_child(const cmd& command, char **arlist,
    const bool &flag_irdir, const bool &flag_ordir,
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
    if (-1==execvp(command.get_exec(),arlist)) {
        perror("execvp");
        return false;
    }
    return true;
}

bool restore_fds(const bool &flag_irdir, const bool &flag_ordir,
    const int &sstdin, const int &sstdout) {
    if (flag_irdir) {
        if (-1==dup2(sstdin,0)) {
            perror("dup2");
            return false;
        }
    }
    if (flag_ordir) {
        if (-1==dup2(sstdout,1)) {
            perror("dup2");
            return false;
        }
    }
    return true;
}

bool close_fds(const bool &flag_irdir, const bool &flag_ordir,
    const int &fdi, const int &fdo) {
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
    std::vector<std::string> v=command.get_arlist();
    char **arlist=new char*[v.size()+1];
    for (std::size_t i=0;i<v.size();i++) {
        arlist[i]=new char[v.at(i).length()+1];
        strcpy(arlist[i],v.at(i).c_str());
    }
    arlist[v.size()]=NULL;
    int fdi,fdo;
    bool flag_irdir=false,flag_ordir=false;
    if (strcmp(command.get_ifile(),"")!=0) {
        flag_irdir=true;
        if (-1==(fdi=open(command.get_ifile(),O_RDONLY))) {
            perror("open");
            return false;
        }
    }
    if (command.get_ordir()!="") {
        flag_ordir=true;
        int flags=command.get_ordir()==">"? O_WRONLY|O_CREAT:
            O_WRONLY|O_CREAT|O_APPEND;
        if (-1==(fdo=open(command.get_ofile(),flags,S_IRUSR|S_IWUSR))) {
            perror("open");
            return false;
        }
    }
    int status=0;
    pid_t pid=fork();
    if (pid<0) {
        perror("fork");
        exit(1);
    }
    else if (pid==0) {
        if (!execute_child(command,arlist,flag_irdir,flag_ordir,fdi,fdo))
            exit(1);
        else exit(0);
    }
    int sstdin,sstdout;
    if (-1==(sstdin=dup(0))) {
        perror("dup");
        exit(1);
    }
    if (-1==(sstdout=dup(1))) {
        perror("dup");
        exit(1);
    }
    if (-1==waitpid(-1,&status,0)) {
        perror("wait");
        exit(1);
    }
    if (!restore_fds(flag_irdir,flag_ordir,sstdin,sstdout))
        exit(1);
    if (!close_fds(flag_irdir,flag_ordir,fdi,fdo))
        exit(1);
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
