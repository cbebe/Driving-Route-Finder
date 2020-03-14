# ---------------------------------------------------
# Name : Charles Ancheta and Poulomi Ganguly
# CMPUT 275 , Winter 2020
# 
# Assignment 2, Part 1: Driving Route Finder (server)
# ---------------------------------------------------

CC := g++
EXE := server
# selects all the files that end with .cpp
SRC := $(wildcard src/*.cpp)
# replaces the .cpp with .o
OBJ := $(SRC:src/%.cpp=obj/%.o)

CFLAGS := -Wall -O2 -std=c++11 -Iinclude

all: $(EXE)

# $< - first subtarget
# $^ - all subtargets
# $@ - self/target name

# links all objects into the executable server
$(EXE): $(OBJ)
	$(CC) $^ -o $@

# compiles all source files from src/ into object files
# and places them in the obj/ directory
obj/%.o: src/%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@ 

# removes all object files and the executable
clean:
	@rm $(OBJ) $(EXE)