#ifndef CMD_H
#define CMD_H

class cmd {
    public:
    cmd() : executable(""), argumentList(nullptr), connector("") {}
    cmd(const std::string exec) : executable(exec) {}
    cmd(const std::string exec, char **arList) : executable(exec) {
        delete[] argumentList;
        argumentList=arList;
    }
    cmd(const std::string exec, const std::string conn) : executable(exec),
        connector(conn) {}
    cmd(const std::string exec, char **arList, const std::string conn) :
    executable(exec), connector(conn) {
        delete[] argumentList;
        argumentList=arList;
    }
    ~cmd() { delete[] argumentList; }

    const std::string get_exec() { return executable; }
    char **get_arList() { return argumentList; }
    const std::string get_conn() { return connector; }

    private:
    std::string executable;
    char **argumentList;
    std::string connector;
};

#endif
