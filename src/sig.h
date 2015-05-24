#ifndef SIG_H
#define SIG_H

#include <errno.h>
#include <queue>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "cmd.h"
#include "login.h"
#include "parse.h"
#include "execute.h"

std::queue<cmd> *commands_ptr;

void handler(int signum) {
    if (signum==SIGINT) {
        std::cerr<<"\n";
        if (!waiting)
            login(get_current_dir_name());
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
