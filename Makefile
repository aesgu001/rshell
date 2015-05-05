ORSHELL = src/main.cpp src/cmd.h src/login.h src/parse.h src/execute.h

CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell ls

rshell: $(ORSHELL)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/rshell src/main.cpp

ls: rshell src/ls.cpp
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/ls src/ls.cpp

cp: rshell src/cp.cpp src/Timer.h
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/cp src/cp.cpp

clean:
	rm -rf bin
