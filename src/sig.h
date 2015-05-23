#ifndef SIG_H
#define SIG_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <queue>
#include "cmd.h"
#include "login.h"
#include "parse.h"

std::queue<cmd> *commands_ptr;
bool waiting=false;

void handler(int signum) {
    if (signum==SIGINT) {
        std::cerr<<"\n";
        if (!waiting)
            login();
        parse_dumpqueue(*commands_ptr);
    }
}

bool sig_init(int signum, struct sigaction &act) {
    if (-1==sigaction(signum,&act,NULL)) {
        perror("sigaction");
        return false;
    }
    return true;
}

#endif
