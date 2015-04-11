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

    private:
    std::string executable;
    char **argumentList;
    std::string connector;
};

#endif
