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
    cmd(const std::string &exec) : executable(exec) {}
    cmd(const std::string &exec, char **arlist) : executable(exec) {
        if (argument_list!=nullptr) delete[] argument_list;
        argument_list=arlist;
    }
    cmd(const std::string &exec, const std::string &conn) :
        executable(exec), argument_list(nullptr), connector(conn) {}
    cmd(const std::string &exec, char **arlist, const std::string &conn) :
        executable(exec), connector(conn) {
        if (argument_list!=nullptr) delete[] argument_list;
        argument_list=arlist;
    }
    ~cmd() { delete[] argument_list; }

    const std::string get_exec() const { return executable; }
    char **get_arlist() const { return argument_list; }
    const std::string get_conn() const { return connector; }

    void set_exec(const std::string exec) { executable=exec; }
    void set_conn(const std::string conn) { connector=conn; }

    void print() const {
        std::cout << "executable: " << executable << std::endl;
        std::cout << "arguments: ";
        for (char **curr=argument_list;curr!=0;curr++)
            std::cout << curr[0] << " ";
        std::cout << std::endl;
        std::cout << "connector: ";
        if (connector=="") std::cout << "none" << std::endl;
        else std::cout << connector << std::endl;
    }
};

#endif
