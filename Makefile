CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell ls

rshell: src/main.cpp src/cmd.h src/login.h
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o bin/rshell src/main.cpp

ls: rshell src/ls.cpp
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o bin/ls src/ls.cpp

cp: rshell src/cp.cpp src/Timer.h
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o bin/cp src/cp.cpp
