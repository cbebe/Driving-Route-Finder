CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=
PROGS= server
OBJS= server.o digraph.o readgraph.o dijkstra.o

server: server.o dijkstra.o readgraph.o digraph.o
	$(CC) $(OBJS) -o $(PROGS) $(LFLAGS)

server.o: server.cpp
	$(CC) server.cpp -o server.o $(CFLAGS)

dijkstra.o: dijkstra.cpp digraph.h wdigraph.h heap.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

readgraph.o: readgraph.cpp 
	$(CC) readgraph.cpp -o readgraph.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)