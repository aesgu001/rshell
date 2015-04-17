CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell

git status:

rshell: src/main.cpp src/cmd.h
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o bin/rshell src/main.cpp
