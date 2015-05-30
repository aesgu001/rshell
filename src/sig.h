#ifndef SIG_H
#define SIG_H

#include <queue>
#include <signal.h>
#include <stack>
#include <stdio.h>
#include <unistd.h>
#include "cmd.h"
#include "login.h"
#include "misc.h"

struct job {
    job(): name(""), pid(0), running(false) {}
    job(const std::string &n, const pid_t &p, const bool &r):
        name(n), pid(p), running(r) {}
    std::string name;
    pid_t pid;
    bool running;
};

bool waiting=false;
pid_t *pid_ptr;
std::queue<cmd> *cmds_ptr;
struct sigaction act;
std::stack<job> jobs;

bool bg(const char *arg0) {
    if (jobs.empty()) {
        std::cerr<<arg0<<": bg: current: no such job.\n";
        return false;
    }
    if (jobs.top().running)
        std::cerr<<arg0<<": bg: job already in background\n";
    else {
        if (-1==kill(jobs.top().pid,SIGCONT)) {
            perror("kill");
            exit(1);
        }
        std::cerr<<"+ "<<jobs.top().name<<" &\n";
        jobs.top().running=true;
    }
    return true;
}

bool fg(const char *arg0) {
    if (jobs.empty()) {
        std::cerr<<arg0<<": fg: current: no such job.\n";
        return false;
    }
    int status=0;
    job j=jobs.top();
    pid_ptr=&j.pid;
    jobs.pop();
    waiting=true;
    std::cerr<<j.name<<"\n";
    if (-1==kill(*pid_ptr,SIGCONT)) {
        perror("kill");
        exit(1);
    }
    if (-1==(waitpid(*pid_ptr,&status,WUNTRACED))) {
        perror("waitpid");
        exit(1);
    }
    waiting=false;
    if (WIFSTOPPED(status)) {
        std::cerr<<"+  Stopped";
        std::cerr<<"                "<<j.name<<"\n";
        jobs.top().name=j.name;
    }
    return status==0? true:false;
}

void handler(int signum) {
    if (signum==SIGINT) {
        std::cerr<<"\n";
        if (!waiting)
            login(getenv("PWD"));
        else
            if (-1==kill(*pid_ptr,SIGKILL)) {
                perror("kill");
                exit(1);
            }
        dump_queue(*cmds_ptr);
    }
    else if (signum==SIGTSTP) {
        if (waiting) {
            std::cerr<<"\n";
            if (-1==kill(*pid_ptr,SIGSTOP)) {
                perror("kill");
                exit(1);
            }
            jobs.push(job("",*pid_ptr,false));
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
