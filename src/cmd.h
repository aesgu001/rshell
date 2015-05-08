#ifndef CMD_H
#define CMD_H

#include <string.h>

class cmd {
    private:
    std::string executable;
    char **argument_list;
    std::size_t arlist_sz;
    std::size_t arlist_cap;
    std::string connector;

    void expand() {
        arlist_cap=arlist_sz;
        arlist_cap*=2;
        char **temp=argument_list;
        argument_list=new char*[arlist_cap];
        if (temp==NULL) return;
        for (std::size_t i=0;i<arlist_sz-1;i++) {
            if (temp[i]==NULL)
                argument_list[i]=NULL;
            else {
                argument_list[i]=new char[strlen(temp[i])+1];
                strcpy(argument_list[i],temp[i]);
            }
            delete[] temp[i];
        }
        delete[] temp;
    }

    public:
    cmd(): executable(""), argument_list(NULL), arlist_sz(0),
        arlist_cap(0), connector("") {}
    ~cmd() {}

    const std::string get_exec() const { return executable; }
    char **get_arlist() const { return argument_list; }
    const std::string get_conn() const { return connector; }

    void set_exec(const std::string &exec) { executable=exec; }
    void set_conn(const std::string &conn) { connector=conn; }
    void push(const char *s) {
        arlist_sz++;
        if (arlist_sz>arlist_cap) expand();
        if (s==NULL)
            argument_list[arlist_sz-1]=NULL;
        else {
            argument_list[arlist_sz-1]=new char[strlen(s)+1];
            strcpy(argument_list[arlist_sz-1],s);
        }
    }
};

#endif
