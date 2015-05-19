#include <iostream>
#include "file.h"
#include "ls.h"
using namespace std;

void get_ff(int argc, char **argv, vector<file> &ls, bool &flag_a,
    bool &flag_l, bool &flag_R) {
    string s;
    for (int i=1;i<argc;i++) {
        s=argv[i];
        if (s.at(0)=='-') {
            if (!flag_a&&s.find('a')!=string::npos) flag_a=true;
            if (!flag_l&&s.find('l')!=string::npos) flag_l=true;
            if (!flag_R&&s.find('R')!=string::npos) flag_R=true;
        }
        else ls.push_back(file(s,s));
    }
    if (!ls.empty())
        sort(ls.begin(),ls.end());
}

int main(int argc, char **argv) {
    vector<file> ls;
    bool flag_a=false,flag_l=false,flag_R=false;
    get_ff(argc,argv,ls,flag_a,flag_l,flag_R);
    list(ls,flag_a,flag_l,flag_R);
    return 0;
}
