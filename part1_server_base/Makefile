# ---------------------------------------------------
# Name : Charles Ancheta and Poulomi Ganguly
# CMPUT 275 , Winter 2020
# 
# Assignment 2, Part 1: Driving Route Finder (server)
# ---------------------------------------------------

CC := g++
# selects all the files that end with .cpp in src/
SRC := $(wildcard src/*.cpp)
# replaces the .cpp with .o
OBJ := $(SRC:src/%.cpp=obj/%.o)

# -Iinclude flag includes all header files 
# in include/ directory in the compilation process
CFLAGS := -Wall -O2 -std=c++11 -Iinclude

# links all objects into the executable server
server: obj | $(OBJ)
	$(CC) $(OBJ) -o server

# compiles all source files from src/ into object files
# and places them in the obj/ directory
obj/server.o: src/server.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 
	
obj/dijkstra.o: src/dijkstra.cpp 
	$(CC) $(CFLAGS) -c $< -o $@

obj/digraph.o: src/digraph.cpp
	$(CC) $(CFLAGS) -c $< -o $@

obj/readgraph.o: src/readgraph.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# creates obj directory if there isn't one already
obj:
	mkdir obj
# removes all object files, obj directory, and the executable
clean:
	@rm $(OBJ) server
	@rmdir obj