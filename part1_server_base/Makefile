CC := g++
EXE := server
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=obj/%.o)

CFLAGS := -Wall -O2 -std=c++11 -Iinclude

all: $(EXE)

# links all objects into the executable 
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@

# compiles all source files into object files
obj/%.o: src/%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	@rm $(OBJ) $(EXE)