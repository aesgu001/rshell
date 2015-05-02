CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell cp

rshell: src/main.cpp src/cmd.h
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o bin/rshell src/main.cpp

cp: src/cp.cpp src/Timer.h
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o bin/cp src/cp.cpp
