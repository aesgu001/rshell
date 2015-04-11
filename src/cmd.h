#ifndef CMD_H
#define CMD_H

class cmd {
    public:
    cmd() : executable(nullptr), argumentlist(nullptr), connector("") {}
    cmd(char *exec) : executable(exec) {}
    cmd(char *exec, char **arList) : executable(exec),
        argumentlist(arList) {}
    cmd(char *exec, const std::string conn) : executable(exec),
        connector(conn) {}
    cmd(char *exec, char **arList, const std::string conn) :
        executable(exec), argumentlist(arList), connector(conn) {}

    char *get_exec() { return executable; }
    char **get_arList() { return argumentlist; }
    const std::string get_conn() { return connector; }

    private:
    char *executable;
    char **argumentlist;
    std::string connector;
};

#endif
