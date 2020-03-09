CC := g++

EXE := server

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CFLAGS := -Wall -O2 -std=c++11

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm $(OBJ) $(EXE)