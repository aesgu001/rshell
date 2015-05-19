#ifndef FILE_H
#define FILE_H

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct file {
    file(): name(""), nname(""), nil(false) {}
    file(const std::string &n, const std::string &nn): name(n), nname(nn),
        nil(false) {
        if (-1==stat(name.c_str(),&buf)) {
            std::string serr="ls: cannot access "+nname;
            perror(serr.c_str());
            nil=true;
        }
    }
    const bool operator<(const file &rhs) const {
        return nname<rhs.nname;
    }
    struct stat buf;
    std::string name,nname;
    bool nil;
};

bool is_dir(const file &f) {
    return f.buf.st_mode&S_IFDIR;
}

bool is_exec(const file &f) {
    return f.buf.st_mode&S_IXUSR;
}

bool is_currdir(const file &f) {
    return f.nname=="."||f.nname=="..";
}

#endif
