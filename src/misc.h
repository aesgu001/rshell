#ifndef MISC_H
#define MISC_H

#include <queue>

template<typename T>
void dump_queue(std::queue<T> &q) {
    while (!q.empty())
        q.pop();
}

#endif
