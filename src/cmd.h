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

    const std::string get_exec() {
        return executable;
    }

    const std::string get_conn() {
        return connector;
    }

    bool match(const std::string str) {
        for (int i=0;i<argumentList.size();i++)
            if (argumentList.at(i)==str)
                return true;
        return false;
    }

    private:
    std::string executable;
    std::vector<std::string> argumentList;
    std::string connector;
};

#endif
