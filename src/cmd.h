#ifndef CMD_H
#define CMD_H

#include <string.h>
#include <vector>

class cmd {
    private:
    std::string exec;
    std::vector<std::string> arlist;
    std::string conn;

    public:
    cmd(): exec(""), arlist(std::vector<std::string>()), conn("") {}

    const std::string get_executable() const { return exec; }
    const std::vector<std::string> get_arlist() const { return arlist; }
    const std::string get_connector() const { return conn; }

    void set_executable(const std::string &e) { exec=e; }
    void set_connector(const std::string &c) { conn=c; }
    void push(const std::string &s) { arlist.push_back(s); }
};

#endif
