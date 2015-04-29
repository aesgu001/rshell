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
        if (-1==stat(name.c_str(),&buf)) {
            perror("stat");
        }
    }
    const bool operator<(const file &rhs) const {
        return name>rhs.name;
    }
    struct stat buf;
    string name;
};

void handle_files_flags(char **argv, priority_queue<file> &flist,
    bool &flag_a, bool &flag_l, bool &flag_R) {
    string s;
    for(size_t i=1;argv[i]!=NULL;i++) {
        s=argv[i];
        if (s.at(0)=='-') {
            if (s.find('a')!=string::npos&&!flag_a) flag_a=true;
            if (s.find('l')!=string::npos&&!flag_l) flag_l=true;
            if (s.find('R')!=string::npos&&!flag_R) flag_R=true;
        }
        else
            flist.push(file(argv[i]));
    }
}

bool is_current_dir(const file &f) {
    return f.name=="."||f.name=="..";
}

priority_queue<file> get_dirs(priority_queue<file> &flist) {
    priority_queue<file> ret;
    while (!flist.empty()) {
        if (flist.top().buf.st_mode & S_IFDIR&&!is_current_dir(flist.top()))
            ret.push(flist.top());
        flist.pop();
    }
    return ret;
}

void execute_print_l(priority_queue<file> &list) {
    while (!list.empty()) {
        cout<<list.top().name<<endl;
        list.pop();
    }
}

void execute_print(const priority_queue<file> &flist, const bool &flag_l) {
    priority_queue<file> list=flist;
    if (flag_l) { execute_print_l(list); return; }
    while (!list.empty()) {
        cout<<list.top().name<<"  ";
        list.pop();
    }
    cout<<endl;
}

void execute_help(const char *s, const bool &flag_a, const bool &flag_l,
    const bool &flag_R) {
    priority_queue<file> fsublist;
    DIR *dirp;
    if (NULL==(dirp=opendir(s))) { perror("opendir"); exit(1); }
    struct dirent *filespecs;
    errno=0;
    while (NULL!=(filespecs=readdir(dirp))) {
        if (!flag_a) {
            if (*filespecs->d_name!='.')
                fsublist.push(file(filespecs->d_name));
        }
        else fsublist.push(file(filespecs->d_name));
    }
    if (errno!=0) { perror("readdir"); exit(1); }
    if (-1==closedir(dirp)) { perror("closedir"); exit(1); }
    if (flag_R) {
        cout<<s<<":"<<endl;
        execute_print(fsublist,flag_l);
        fsublist=get_dirs(fsublist);
        while (!fsublist.empty()) {
             execute_help(fsublist.top().name.c_str(),flag_a,flag_l,flag_R);
             fsublist.pop();
        }
    }
    else execute_print(fsublist,flag_l);
}

void execute(priority_queue<file> &flist, const bool &flag_a, const bool &flag_l,
    const bool &flag_R) {
    if (flist.empty()) { execute_help(".",flag_a,flag_l,flag_R); return; }
    while (!flist.empty()) {
        if (flist.top().buf.st_mode & S_IFDIR)
            execute_help(flist.top().name.c_str(),flag_a,flag_l,flag_R);
        flist.pop();
    }
}

int main(int argc, char **argv) {
    priority_queue<file> flist;
    bool flag_a=false,flag_l=false,flag_R=false;
    handle_files_flags(argv,flist,flag_a,flag_l,flag_R);
    execute(flist,flag_a,flag_l,flag_R);
    return 0;
}
