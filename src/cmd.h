#ifndef CMD_H
#define CMD_H

#include <vector>

//struct argumentList {
//    argumentList() {}
//    argumentList(std::string arg) : argument(arg) {}
//    argumentList *next;
//    std::string argument;
//};

class cmd {
    public:
    cmd() {}
    cmd(const std::string exec, const std::vector<std::string> argList,
        const std::string conn) : executable(exec), argumentList(argList),
        connector(conn) {}

    const std::string get_exec() { return executable; }

    void set_exec(const std::string str) { executable=str; }

    const std::string get_conn() { return connector; }

    void set_conn(const std::string str) { connector=str; }

    private:
    std::string executable;
    std::vector<std::string> argumentList;
    std::string connector;
};

#endif
