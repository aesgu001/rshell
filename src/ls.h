#ifndef LS_H
#define LS_H

#include <algorithm>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <iomanip>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "col.h"
#include "file.h"

void list_print_l_total(const std::vector<file> &ls) {
    blkcnt_t blksz=0;
    for (std::size_t i=0;i<ls.size();i++)
        blksz+=ls.at(i).buf.st_blocks;
    std::cout<<"total "<<static_cast<std::size_t>(blksz/2)<<std::endl;
}

void list_print_l_pms(const file &f) {
    f.buf.st_mode&S_IFDIR? std::cout<<"d":std::cout<<"-";
    f.buf.st_mode&S_IRUSR? std::cout<<"r":std::cout<<"-";
    f.buf.st_mode&S_IWUSR? std::cout<<"w":std::cout<<"-";
    f.buf.st_mode&S_IXUSR? std::cout<<"x":std::cout<<"-";
    f.buf.st_mode&S_IRGRP? std::cout<<"r":std::cout<<"-";
    f.buf.st_mode&S_IWGRP? std::cout<<"w":std::cout<<"-";
    f.buf.st_mode&S_IXGRP? std::cout<<"x":std::cout<<"-";
    f.buf.st_mode&S_IROTH? std::cout<<"r":std::cout<<"-";
    f.buf.st_mode&S_IWOTH? std::cout<<"w":std::cout<<"-";
    f.buf.st_mode&S_IXOTH? std::cout<<"x":std::cout<<"-";
}

void list_print_l(const std::vector<file> &ls, const bool &count) {
    struct passwd *usr;
    struct group *grp;
    std::string stime;
    if (count)
        list_print_l_total(ls);
    for (std::size_t i=0;i<ls.size();i++) {
        list_print_l_pms(ls.at(i));
        std::cout<<" ";
        std::cout<<ls.at(i).buf.st_nlink<<" ";
        if (NULL==(usr=getpwuid(ls.at(i).buf.st_uid))) {
            perror("getpwuid");
            exit(1);
        }
        std::cout<<usr->pw_name<<" ";
        if (NULL==(grp=getgrgid(ls.at(i).buf.st_gid))) {
            perror("getgrgid");
            exit(1);
        }
        std::cout<<grp->gr_name<<" ";
        std::cout<<std::setw(5)<<ls.at(i).buf.st_size<<" ";
        stime=ctime(&ls.at(i).buf.st_mtime);
        stime=stime.substr(4,12);
        std::cout<<stime<<" ";
        if (is_dir(ls.at(i)))
            std::cout<<col_blue<<ls.at(i).nname<<col_def<<std::endl;
        else if (is_exec(ls.at(i)))
            std::cout<<col_green<<ls.at(i).nname<<col_def<<std::endl;
        else std::cout<<ls.at(i).nname<<std::endl;
    }
}

void list_print(const std::vector<file> &ls) {
    for (std::size_t i=0;i<ls.size();i++) {
        if (is_dir(ls.at(i)))
            std::cout<<col_blue<<ls.at(i).nname<<col_def<<"  ";
        else if (is_exec(ls.at(i)))
            std::cout<<col_green<<ls.at(i).nname<<col_def<<"  ";
        else std::cout<<ls.at(i).nname<<"  ";
    }
    std::cout<<std::endl;
}

std::vector<file> list_getdirs(const std::vector<file> &ls) {
    std::vector<file> subls;
    for (std::size_t i=0;i<ls.size();i++)
        if (is_dir(ls.at(i))&&!is_currdir(ls.at(i)))
            subls.push_back(ls.at(i));
    if (!subls.empty())
        sort(subls.begin(),subls.end());
    return subls;
}

void list_help(const file &f, const bool &flag_a, const bool &flag_l,
    const bool &flag_R, const bool &flag_RR) {
    std::vector<file> subls;
    DIR *dirp;
    struct dirent *filespecs;
    errno=0;
    std::string pdir=f.name+"/";
    file ff;
    if (NULL==(dirp=opendir(f.name.c_str()))) {
        perror("opendir");
        exit(1);
    }
    while (NULL!=(filespecs=readdir(dirp))) {
        ff=file(pdir+filespecs->d_name,filespecs->d_name);
        if (ff.nil)
            continue;
        else if (!flag_a) {
            if (ff.nname.at(0)!='.')
                subls.push_back(ff);
        }
        else subls.push_back(ff);
    }
    if (errno!=0) {
        perror("readdir");
        exit(1);
    }
    if (-1==closedir(dirp)) {
        perror("closedir");
        exit(1);
    }
    if (subls.empty())
        return;
    sort(subls.begin(),subls.end());
    if (flag_R) {
        if (!flag_RR)
            std::cout<<f.name<<":"<<std::endl;
        flag_l? list_print_l(subls,true):list_print(subls);
        subls=list_getdirs(subls);
        if (!subls.empty())
            std::cout<<std::endl;
        for (std::size_t i=0;i<subls.size();i++) {
            list_help(subls.at(i),flag_a,flag_l,flag_R,false);
            if (i+1<subls.size())
                std::cout<<std::endl;
        }
    }
    else flag_l? list_print_l(subls,true):list_print(subls);
}

void list(const std::vector<file> &ls, const bool &flag_a, const bool &flag_l,
    const bool &flag_R) {
    if (ls.empty()) {
        list_help(file(".","."),flag_a,flag_l,flag_R,false);
        return;
    }
    std::vector<file> lsf,lsd;
    for (int i=ls.size()-1;i>=0;i--) {
        if (!ls.at(i).nil)
            is_dir(ls.at(i))? lsd.push_back(ls.at(i)):
                lsf.push_back(ls.at(i));
    }
    if (!lsf.empty()) {
        sort(lsf.begin(),lsf.end());
        flag_l? list_print_l(lsf,false):list_print(lsf);
    }
    if (!lsd.empty()) {
        if (!lsf.empty())
            std::cout<<std::endl;
        sort(lsd.begin(),lsd.end());
        for (std::size_t i=0;i<lsd.size();i++) {
            if (lsd.size()>1)
                std::cout<<ls.at(i).name<<":"<<std::endl;
            list_help(ls.at(i),flag_a,flag_l,flag_R,true);
            if (i+1<lsd.size())
                std::cout<<std::endl;
        }
    }
}

#endif
