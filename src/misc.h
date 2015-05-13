#ifndef MISC_H
#define MISC_H

#include <queue>

void print_error_token(const char *arg0, const char *tok) {
    std::cout<<arg0<<": syntax error near unexpected token `"<<tok<<"'"
        <<std::endl;
}

template <typename T>
void dump_queue(std::queue<T> &q) {
    while (!q.empty())
        q.pop();
}

#endif
