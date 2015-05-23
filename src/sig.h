#ifndef SIG_H
#define SIG_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include "login.h"

void handler(int signum) {
    if (signum==SIGINT) {
        std::cerr<<"\n";
        login();
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
