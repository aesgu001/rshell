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

void handle_files_flags(char **argv, priority_queue<file> &list,
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
            list.push(file(argv[i]));
    }
}

bool is_dir(const file &f) {
    return f.buf.st_mode&S_IFDIR;
}

bool is_current_dir(const file &f) {
    return f.name=="."||f.name=="..";
}

priority_queue<file> get_dirs(const priority_queue<file> &list) {
    priority_queue<file> ret,temp=list;
    while (!temp.empty()) {
        if (is_dir(temp.top())&&!is_current_dir(temp.top())) {
            ret.push(temp.top());
        }
        temp.pop();
    }
    return ret;
}

void print_l_pms(const file &f) {
    f.buf.st_mode&S_IFDIR? cout<<"d":cout<<"-";
    f.buf.st_mode&S_IRUSR? cout<<"r":cout<<"-";
    f.buf.st_mode&S_IWUSR? cout<<"w":cout<<"-";
    f.buf.st_mode&S_IXUSR? cout<<"x":cout<<"-";
    f.buf.st_mode&S_IRGRP? cout<<"r":cout<<"-";
    f.buf.st_mode&S_IWGRP? cout<<"w":cout<<"-";
    f.buf.st_mode&S_IXGRP? cout<<"x":cout<<"-";
    f.buf.st_mode&S_IROTH? cout<<"r":cout<<"-";
    f.buf.st_mode&S_IWOTH? cout<<"w":cout<<"-";
    f.buf.st_mode&S_IXOTH? cout<<"x":cout<<"-";
}

void execute_print_l(const priority_queue<file> &list) {
    priority_queue<file> temp=list;
    while (!temp.empty()) {
        print_l_pms(temp.top());
        cout<<" ";
        cout<<temp.top().name<<endl;
        temp.pop();
    }
}

void execute_print(const priority_queue<file> &list) {
    priority_queue<file> temp=list;
    while (!temp.empty()) {
        cout<<temp.top().name<<"  ";
        temp.pop();
    }
    cout<<endl;
}

void execute_help(const file &f, const bool &flag_a, const bool &flag_l,
    const bool &flag_R) {
    priority_queue<file> sublist;
    DIR *dirp;
    if (NULL==(dirp=opendir(f.name.c_str()))) { perror("opendir"); exit(1); }
    struct dirent *filespecs;
    errno=0;
    while (NULL!=(filespecs=readdir(dirp))) {
        if (!flag_a) {
            if (*filespecs->d_name!='.')
                sublist.push(file(filespecs->d_name));
        }
        else sublist.push(file(filespecs->d_name));
    }
    if (errno!=0) { perror("readdir"); exit(1); }
    if (-1==closedir(dirp)) { perror("closedir"); exit(1); }
    if (flag_R) {
        cout<<f.name<<":"<<endl;
        flag_l? execute_print_l(sublist):execute_print(sublist);
        sublist=get_dirs(sublist);
        while (!sublist.empty()) {
             execute_help(sublist.top(),flag_a,flag_l,flag_R);
             sublist.pop();
        }
    }
    else flag_l? execute_print_l(sublist):execute_print(sublist);
}

void execute(priority_queue<file> &list, const bool &flag_a, const bool &flag_l,
    const bool &flag_R) {
    if (list.empty()) { execute_help(file("."),flag_a,flag_l,flag_R); return; }
    while (!list.empty()) {
        if (is_dir(list.top()))
            execute_help(list.top().name.c_str(),flag_a,flag_l,flag_R);
        list.pop();
    }
}

int main(int argc, char **argv) {
    priority_queue<file> list;
    bool flag_a=false,flag_l=false,flag_R=false;
    handle_files_flags(argv,list,flag_a,flag_l,flag_R);
    execute(list,flag_a,flag_l,flag_R);
    return 0;
}
