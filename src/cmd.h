#ifndef CMD_H
#define CMD_H

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
};

#endif
