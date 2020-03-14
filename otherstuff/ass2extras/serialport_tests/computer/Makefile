OBJS = serialtest.o serialport.o
EXE = serialtest
CFLAGS= -c -Wall -std=c++11

serialtest: $(OBJS)
	g++ $(OBJS) -o $(EXE)

serialtest.o: serialport.h serialtest.cpp
	g++ serialtest.cpp -o serialtest.o $(CFLAGS)

serialport.o: serialport.h serialport.cpp
	g++ serialport.cpp -o serialport.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(EXE)
