CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=
PROGS= digraph_test dijkstra
OBJS= digraph.o digraph_test.o dijkstra.o

# executable targets
all: digraph_test dijkstra

dijkstra: dijkstra.o digraph.o
	$(CC) dijkstra.o digraph.o -o dijkstra $(LFLAGS)

digraph_test: digraph_test.o digraph.o
	$(CC) digraph_test.o digraph.o -o digraph_test $(LFLAGS)

# object targets
dijkstra.o: dijkstra.cpp digraph.h wdigraph.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

digraph_test.o: digraph_test.cpp digraph.h wdigraph.h
	$(CC) digraph_test.cpp -o digraph_test.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)
