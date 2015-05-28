#ifndef CD_H
#define CD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cmd.h"

bool cd_help(const char *pwd) {
    if (-1==chdir(pwd)) {
        perror("chdir");
        return false;
    }
    if (-1==setenv("OLDPWD",getenv("PWD"),1)) {
        perror("setenv");
        return false;
    }
    char *cdir=get_current_dir_name();
    if (-1==setenv("PWD",cdir,1)) {
        perror("setenv");
        free(cdir);
        return false;
    }
    free(cdir);
    return true;
}

bool cd(const cmd &command) {
    if (command.get_arlist().size()<=1||
        command.get_arlist().at(1)=="~") {
        if (!cd_help(getenv("HOME")))
            return false;
    }
    else if (command.get_arlist().at(1)=="-") {
        std::cerr<<getenv("OLDPWD")<<"\n";
        if (!cd_help(getenv("OLDPWD")))
            return false;
    }
    else {
        if (!cd_help(command.get_arlist().at(1).c_str()))
            return false;
    }
    return true;
}

#endif
