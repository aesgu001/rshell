BIN = mkdir -p bin
OCP = src/cp.cpp src/Timer.h
OLS = src/ls.cpp src/col.h src/file.h src/ls.h
ORSHELL = src/main.cpp src/cd.h src/cmd.h src/execute.h src/login.h src/misc.h src/parse.h src/sig.h

CXX = g++
CPPFLAGS = -std=c++11
DFLAGS = -Wall -Werror -ansi -pedantic

all: rshell

rshell: $(ORSHELL)
	$(BIN)
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/rshell src/main.cpp

ls: $(OLS)
	$(BIN)
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/ls src/ls.cpp

cp: $(OCP)
	$(BIN)
	$(CXX) $(CPPFLAGS) $(DFLAGS) -g -o bin/cp src/cp.cpp

clean:
	rm -rf bin
