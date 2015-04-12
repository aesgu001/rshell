#ifndef CMD_H
#define CMD_H

#include <iostream>

class cmd {
    private:
    std::string executable;
    char **argument_list;
    std::string connector;

    public:
    cmd() : executable(""), argument_list(nullptr), connector("") {}
    cmd(const std::string exec) : executable(exec) {}
    cmd(const std::string exec, char **arlist) : executable(exec) {
        char **temp=argument_list;
        argument_list=arlist;
        delete[] temp;
    }
    cmd(const std::string exec, char **arlist, const std::string conn) :
    executable(exec), connector(conn) {
        char **temp=argument_list;
        argument_list=arlist;
        delete[] temp;
    }
    ~cmd() { delete[] argument_list; }

    const std::string get_exec() const { return executable; }
    char **get_arlist() const { return argument_list; }
    const std::string get_conn() const { return connector; }

    void print() const {
        std::cout << "executable: " << executable << std::endl;
        std::cout << "argument list: ";
        for (char **curr=argument_list;curr!=0;curr++)
            std::cout << curr[0] << " ";
        std::cout << std::endl;
        std::cout << "connector: ";
        if (connector=="") std::cout << "none" << std::endl;
        else std::cout << connector << std::endl;
    }
};

#endif
