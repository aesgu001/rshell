#ifndef LOGIN_H
#define LOGIN_H

#include <unistd.h>
#include <stdio.h>

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
