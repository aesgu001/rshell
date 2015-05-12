#ifndef CMD_H
#define CMD_H

#include <vector>

class cmd {
    private:
    std::string exec,conn,ordir,fin,fout;
    std::vector<std::string> arlist;

    public:
    cmd(): exec(""), conn(""), ordir(""), fin(""), fout(""),
        arlist(std::vector<std::string>()) {}

    const char *get_exec() const { return exec.c_str(); }
    const char *get_conn() const { return conn.c_str(); }
    const std::string get_ordir() const { return ordir; }
    const char *get_ifile() const { return fin.c_str(); }
    const char *get_ofile() const { return fout.c_str(); }
    const std::vector<std::string> get_arlist() const { return arlist; }

    void set_exec(const std::string &e) { exec=e; }
    void set_conn(const std::string &c) { conn=c; }
    void set_ordir(const std::string &ord) { ordir=ord; }
    void set_ifile(const std::string &i) { fin=i; }
    void set_ofile(const std::string &o) { fout=o; }
    void push_arg(const std::string &arg) { arlist.push_back(arg); }
};

#endif
