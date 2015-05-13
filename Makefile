BIN = mkdir -p bin
OCP = src/cp.cpp src/Timer.h
ORSHELL = src/main.cpp src/cmd.h src/login.h src/parse.h src/execute.h

CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell ls

rshell: $(ORSHELL)
	$(BIN)
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/rshell src/main.cpp

ls: src/ls.cpp
	$(BIN)
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/ls src/ls.cpp

cp: $(OCP)
	$(BIN)
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/cp src/cp.cpp

clean:
	rm -rf bin
