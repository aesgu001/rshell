#ifndef CMD_H
#define CMD_H

#include <string.h>

class cmd {
    private:
    std::string exec;
    char **arlist;
    std::size_t arlist_sz;
    std::size_t arlist_cap;
    std::string conn;

    void expand() {
        arlist_cap=arlist_sz;
        arlist_cap*=2;
        char **temp=arlist;
        arlist=new char*[arlist_cap];
        if (temp==NULL) return;
        for (std::size_t i=0;i<arlist_sz-1;i++) {
            if (temp[i]==NULL)
                arlist[i]=NULL;
            else {
                arlist[i]=new char[strlen(temp[i])+1];
                strcpy(arlist[i],temp[i]);
            }
            delete[] temp[i];
        }
        delete[] temp;
    }

    public:
    cmd(): exec(""), arlist(NULL), arlist_sz(0),
        arlist_cap(0), conn("") {}
    ~cmd() {}

    const std::string get_executable() const { return exec; }
    char **get_arlist() const { return arlist; }
    const std::string get_connector() const { return conn; }

    void set_executable(const std::string &e) { exec=e; }
    void set_connector(const std::string &c) { conn=c; }
    void push(const char *arg) {
        arlist_sz++;
        if (arlist_sz>arlist_cap) expand();
        if (arg==NULL)
            arlist[arlist_sz-1]=NULL;
        else {
            arlist[arlist_sz-1]=new char[strlen(arg)+1];
            strcpy(arlist[arlist_sz-1],arg);
        }
    }
};

#endif
