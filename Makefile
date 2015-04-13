CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell

rshell: src/main.cpp src/cmd.h
	$(CXX) $(CPPFLAGS) $(DFLAGS) -o rshell src/main.cpp
