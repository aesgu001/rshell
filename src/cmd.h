#ifndef CMD_H
#define CMD_H

class cmd {
    private:
    std::string executable;
    char **argument_list;
    std::size_t arlist_sz;
    std::size_t arlist_cap;
    std::string connector;

    void expand() {
        if (arlist_cap==0) arlist_cap=arlist_sz;
        arlist_cap*=2;
        char **temp=argument_list;
        argument_list=new char*[arlist_cap];
        if (arlist_sz<=1) return;
        for (std::size_t i=0;i<arlist_sz;i++)
            *(argument_list+i)=*(temp+i);
    }

    public:
    cmd() : executable(""), argument_list(NULL), arlist_sz(0),
        arlist_cap(0), connector("") {}
    cmd(const std::string &exec) : executable(exec), argument_list(NULL),
        arlist_sz(0), arlist_cap(0), connector("") {}
    cmd(const std::string &exec, const std::string &conn) :
        executable(exec), argument_list(NULL), arlist_sz(0), arlist_cap(0),
        connector(conn) {}

    const std::string get_exec() const { return executable; }
    char **get_arlist() const { return argument_list; }
    const std::string get_conn() const { return connector; }

    void set_exec(const std::string &exec) { executable=exec; }
    void set_conn(const std::string &conn) { connector=conn; }
    void push(char *s) {
        arlist_sz++;
        if (arlist_sz>arlist_cap) expand();
        *(argument_list+arlist_sz-1)=s;
    }

    void print() const {
        std::cout << "command: " << executable << std::endl;
        std::cout << "arguments: ";
        for (std::size_t i=0;i<arlist_sz;i++)
            std::cout << *(argument_list+i) << " ";
        std::cout << std::endl;
        std::cout << "connector: " << connector<< std::endl;
    }
};

#endif
