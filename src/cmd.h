#ifndef CMD_H
#define CMD_H

#include <vector>

class cmd {
    private:
    std::string executable;
    std::vector<std::string> argument_list;
    std::string connector;

    public:
    cmd() : executable(""), argument_list(std::vector<std::string>()),
        connector("") {}
    cmd(const std::string &exec) : executable(exec),
        argument_list(std::vector<std::string>()), connector("") {}
    cmd(const std::string &exec, const std::string &conn) :
        executable(exec), argument_list(std::vector<std::string>()),
        connector(conn) {}

    const std::string get_exec() const { return executable; }
    const std::vector<std::string> get_arlist() const {
        return argument_list;
    }
    const std::string get_conn() const { return connector; }

    void set_exec(const std::string exec) { executable=exec; }
    void set_conn(const std::string conn) { connector=conn; }
    void push(const std::string &s) { argument_list.push_back(s); }

    void print() const {
        std::cout << "executable: " << executable << std::endl;
        std::cout << "arguments: ";
        for (std::size_t i=0;i<argument_list.size();i++)
            std::cout << argument_list.at(i) << " ";
        std::cout << std::endl;
        std::cout << "connector: ";
        if (connector=="") std::cout << "none" << std::endl;
        else std::cout << connector << std::endl;
    }
};

#endif
