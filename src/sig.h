#ifndef SIG_H
#define SIG_H

#include <queue>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include "cmd.h"
#include "login.h"
#include "misc.h"

bool waiting=false;
pid_t *pid_ptr;
std::queue<cmd> *cmds_ptr;
struct sigaction act;
std::vector<std::pair<pid_t,cmd> > bg_prcs;

bool bg(const char *arg0) {
    if (bg_prcs.empty()) {
        std::cerr<<arg0<<": bg: current: no such job.\n";
        return false;
    }
    std::cerr<<"["<<bg_prcs.size()<<"]+ ";
    std::cerr<<bg_prcs.at(bg_prcs.size()-1).second.get_exec()<<" &\n";
    std::cerr<<"\n["<<bg_prcs.size()<<"]+  Stopped";
    std::cerr<<"                "
        <<bg_prcs.at(bg_prcs.size()-1).second.get_exec()<<"\n";
    return true;
}

bool fg(const char *arg0) {
    if (bg_prcs.empty()) {
        std::cerr<<arg0<<": fg: current: no such job.\n";
        return false;
    }
    int status=0;
    std::pair<pid_t,cmd> p=bg_prcs.at(bg_prcs.size()-1);
    pid_ptr=&p.first;
    bg_prcs.pop_back();
    std::cerr<<p.second.get_exec()<<"\n";
    waiting=true;
    if (-1==kill(p.first,SIGCONT)) {
        perror("kill");
        exit(1);
    }
    if (-1==(waitpid(p.first,&status,WUNTRACED))) {
        perror("waitpid");
        exit(1);
    }
    waiting=false;
    if (WIFSTOPPED(status)) {
        std::cerr<<"["<<bg_prcs.size()<<"]+  Stopped";
        std::cerr<<"                "<<p.second.get_exec()<<"\n";
        bg_prcs.at(bg_prcs.size()-1).second=p.second;
    }
    return status==0? true:false;
}

void handler(int signum) {
    if (signum==SIGINT) {
        std::cerr<<"\n";
        if (!waiting)
            login(getenv("PWD"));
        dump_queue(*cmds_ptr);
    }
    else if (signum==SIGTSTP) {
        if (waiting) {
            std::cerr<<"\n";
            if (-1==kill(*pid_ptr,SIGSTOP)) {
                perror("kill");
                exit(1);
            }
            bg_prcs.push_back(std::pair<pid_t,cmd>(*pid_ptr,cmd()));
        }
    }
}

bool sig_init(std::queue<cmd> &commands) {
    act.sa_handler=handler;
    act.sa_flags=SA_RESTART;
    sigemptyset(&act.sa_mask);
    cmds_ptr=&commands;
    if (-1==sigaction(SIGINT,&act,NULL)) {
        perror("sigaction");
        return false;
    }
    if (-1==sigaction(SIGTSTP,&act,NULL)) {
        perror("sigaction");
        return false;
    }
    return true;
}

#endif
