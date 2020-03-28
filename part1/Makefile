CC=g++
LFLAGS=
CFLAGS=-c -Wall -std=c++11 -O2
OBJS=digraph.o dijkstra.o server.o
EXE=server

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

server.o: digraph.h dijkstra.h server.cpp wdigraph.h
	$(CC) server.cpp -o server.o $(CFLAGS)

dijkstra.o: dijkstra.h dijkstra.cpp heap.h digraph.h wdigraph.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

digraph.o: digraph.h digraph.cpp
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(OBJS) $(EXE)
