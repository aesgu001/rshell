#ifndef CMD_H
#define CMD_H

#include <string.h>
#include <vector>

class cmd {
    private:
    std::string exec;
    std::vector<std::string> arlist;
    std::string conn;
    std::string rdir;

    public:
    cmd(): exec(""), arlist(std::vector<std::string>()), conn(""),
        rdir("") {}

    const std::string get_executable() const { return exec; }
    const std::vector<std::string> get_arlist() const { return arlist; }
    const std::string get_connector() const { return conn; }
    const std::string get_redirection() const { return rdir; }

    void set_executable(const std::string &e) { exec=e; }
    void set_connector(const std::string &c) { conn=c; }
    void push(const std::string &arg) { arlist.push_back(arg); }
    void set_redirection(const std::string &rd) { rdir=rd; }
};

#endif
