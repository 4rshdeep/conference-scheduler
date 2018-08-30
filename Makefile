INCLUDES = -I./include/
SOURCES  =  ./src/main.cpp ./src/Track.cpp ./src/Session.cpp ./src/Conference.cpp ./src/SessionOrganizer.cpp
BIN = ./bin/main

all: ./src/main.cpp ./src/Track.cpp ./src/Session.cpp ./src/Conference.cpp ./src/SessionOrganizer.cpp
	g++ -g -std=c++11 $(INCLUDES) -o3 -o $(BIN) $(SOURCES)

clean: 
	rm bin/*

test: all
	./bin/main ./tests/in.txt ./tests/out.txt

check: 
	java -jar ./tests/formatChecker.jar ./tests/in.txt ./tests/out.txt