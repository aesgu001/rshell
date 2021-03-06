#ifndef LOGIN_H
#define LOGIN_H

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void login(const char *cwd) {
    char *user_host;
    if (NULL==(user_host=getlogin())) {
        perror("getlogin");
        exit(1);
    }
    std::cerr<<user_host<<"@";
    if (-1==gethostname(user_host,HOST_NAME_MAX)) {
        perror("gethostname");
        exit(1);
    }
    std::cerr<<user_host<<":";
    if (strlen(cwd)>=strlen(getenv("HOME"))) {
        std::cerr<<"~";
        std::string home=getenv("HOME"),
            path=cwd;
        std::cerr<<path.substr(path.find(home)+home.length(),
            std::string::npos);
    }
    else
        std::cerr<<cwd;
    std::cerr<<"$ ";
}

#endif
