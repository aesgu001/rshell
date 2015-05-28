#ifndef SIG_H
#define SIG_H

#include <queue>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "cmd.h"
#include "execute.h"
#include "login.h"
#include "misc.h"

std::queue<cmd> *cmds_ptr;
struct sigaction act;

void handler(int signum) {
    if (signum==SIGINT) {
        std::cerr<<"\n";
        if (!waiting)
            login(getenv("PWD"));
        dump_queue(*cmds_ptr);
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
    return true;
}

#endif
