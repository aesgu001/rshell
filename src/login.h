#ifndef LOGIN_H
#define LOGIN_H

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void login() {
    char *user_host;
    if (NULL==(user_host=getlogin())) {
        perror("getlogin");
        exit(1);
    }
    std::cout<<user_host<<"@";
    if (-1==gethostname(user_host,HOST_NAME_MAX)) {
        perror("gethostname");
        exit(1);
    }
    std::cout<<user_host<<"$ ";
}

#endif
