#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <queue>
using namespace std;

struct file {
    file(const char *n): name(n) {
        if (-1==stat(name.c_str(),&bf)) {
            perror("stat");
            exit(1);
        }
    }
    const bool operator<(const file &rhs) const {
        return name>rhs.name;
    }
    struct stat bf;
    string name;
};

void handle_flags(char **argv, bool &flag_a, bool &flag_l, bool &flag_R) {
    string s;
    for(size_t i=0;argv[i]!=NULL&&(!flag_a||!flag_l||!flag_R);i++) {
        s=argv[i];
        if (s.at(0)=='-') {
            if (s.find('a')!=string::npos&&!flag_a) flag_a=true;
            if (s.find('l')!=string::npos&&!flag_l) flag_l=true;
            if (s.find('R')!=string::npos&&!flag_R) flag_R=true;
        }
    }
}

/*void handle_files_dirs(char **argv, priority_queue<dirlist> &dlists) {
    for(size_t i=1;argv[i]!=NULL;i++)
        if (*argv[i]!='-')
            dlists.push(dirlist(argv[i]));
}

void execute_help(priority_queue<dirlist> &dlists, const bool &flag_l) {
    if (flag_l) {} // FIXME
    else {
        while (!dlists.empty()) {
            cout<<dlists.top().name<<"  ";
            dlists.pop();
        }
        cout<<endl;
    }
}

void execute(const char *s, const bool &flag_a, const bool &flag_l,
    const bool &flag_R) {
    priority_queue<dirlist> dlists;
    DIR *dirp;
    if (NULL==(dirp=opendir(s))) { perror("opendir"); exit(1); }
    struct dirent *filespecs;
    errno=0;
    while (NULL!=(filespecs=readdir(dirp))) {
        if (!flag_a) {
            if (*filespecs->d_name!='.')
                dlists.push(dirlist(filespecs->d_name));
        }
        else dlists.push(dirlist(filespecs->d_name));
    }
    if (errno!=0) { perror("readdir"); exit(1); }
    if (-1==closedir(dirp)) { perror("closedir"); exit(1); }
    execute_help(dlists,flag_l);
}*/

int main(int argc, char **argv) {
    /*priority_queue<dirlist> dlists;
    bool flag_a=false,flag_l=false,flag_R=false;
    handle_flags(argv,flag_a,flag_l,flag_R);
    handle_files_dirs(argv,dlists);
    if (dlists.empty()) execute(".",flag_a,flag_l,flag_R);*/
    return 0;
}
