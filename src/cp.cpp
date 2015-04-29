#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Timer.h"

using namespace std;

void method1(char** argv);
void method2(char** argv);
void method3(char** argv);
int main(int argc, char** argv)
{
    if(argc < 3 || argc > 4)
    {
        cout << "Wrong parameters" << endl;
        exit(1);
    }
    else if(argc == 3)
    {
        struct stat buf;
        if(0 == stat(argv[2], &buf))
        {
            cout << "Fail: " << argv[2] << " exists" << endl;
            exit(1);
        }
        //method1(argv);
        //method2(argv);
        method3(argv);
    }
    else
    {
        Timer t;
        double WallclockTime, UserTime, SystemTime;
        t.start();
        method1(argv);
        t.elapsedTime(WallclockTime, UserTime, SystemTime);
        cout << "Wallclock time for method 1 is: " << WallclockTime << endl;
        cout << "User time for method 1 is: " << UserTime << endl;
        cout << "System time for method 1 is: " << SystemTime << endl;
        t.start();
        method2(argv);
        t.elapsedTime(WallclockTime, UserTime, SystemTime);
        cout << "Wallclock time for method 2 is: " << WallclockTime << endl;
        cout << "User time for method 2 is: " << UserTime << endl;
        cout << "System time for method 2 is: " << SystemTime << endl;
        t.start();
        method3(argv);
        t.elapsedTime(WallclockTime, UserTime, SystemTime);
        cout << "Wallclock time for method 3 is: " << WallclockTime << endl;
        cout << "User time for method 3 is: " << UserTime << endl;
        cout << "System time for method 3 is: " << SystemTime << endl;
    }
}

void method1(char** argv)
{
    ifstream ifs(argv[1]);
    if(!ifs.is_open())
    {
        cout << "Error with opening infile" << endl;
        exit(1);
    }
    ofstream ofs(argv[2]);
    if(!ofs.is_open())
    {
        cout << "Error with opening outfile" << endl;
        exit(1);
    }
    char c;
    while(ifs.get(c))
        ofs.put(c);
    ifs.close();
}

void method2(char** argv)
{
    int fdnew;
    int fdold;
    if(-1 == (fdnew = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)))
    {
        perror("open()");
        exit(1);
    }
    if(-1 == (fdold = open(argv[1], O_RDONLY)))
    {
        perror("open()");
        exit(1);
    }
    int size;
    char c;
    if(-1 == (size = read(fdold, &c, sizeof(c))))
    {
        perror("read()");
        exit(1);
    }
    while(size > 0)
    {
        if(-1 == write(fdnew, &c, size))
        {
            perror("write()");
            exit(1);
        }
        if(-1 == (size = read(fdold, &c, sizeof(c))))
        {
            perror("read()");
            exit(1);
        }
    }
    if(-1 == close(fdnew))
    {
        perror("close()");
        exit(1);
    }
    if(-1 == close(fdold))
    {
        perror("close()");
        exit(1);
    }
    return;
}


void method3(char** argv)
{
    int fdnew;
    int fdold;
    if(-1 == (fdnew = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)))
    {
        perror("open()");
        exit(1);
    }
    if(-1 == (fdold = open(argv[1], O_RDONLY)))
    {
        perror("open()");
        exit(1);
    }
    int size;
    char c[BUFSIZ];
    if(-1 == (size = read(fdold, c, sizeof(c))))
    {
        perror("read()");
        exit(1);
    }
    while(size > 0)
    {
        if(-1 == write(fdnew, c, size))
        {
            perror("write()");
            exit(1);
        }
        if(-1 == (size = read(fdold, c, sizeof(c))))
        {
            perror("read()");
            exit(1);
        }
    }
    if(-1 == close(fdnew))
    {
        perror("close()");
        exit(1);
    }
    if(-1 == close(fdold))
    {
        perror("close()");
        exit(1);
    }
    return;
}
