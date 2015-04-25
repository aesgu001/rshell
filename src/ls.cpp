#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
using namespace std;

int main(int argc, char **argv) {
    DIR *dirp;
    if (NULL==(dirp=opendir("."))) { perror("opendir"); exit(1); }
    struct dirent *filespecs;
    errno=0;
    while (NULL!=(filespecs=readdir(dirp)))
        cout<<filespecs->d_name<<"  ";
    if (errno!=0) { perror("readdir"); exit(1); }
    cout<<endl;
    if (-1==closedir(dirp)) { perror("closedir"); exit(1); }
    return 0;
}
