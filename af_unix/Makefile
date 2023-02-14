CPP := g++
CFLAGS := -g -Wall

default: client server

client: client.o
	$(CPP) -o client client.o $(CFLAGS)

server: server.o
	$(CPP) -o server server.o $(CFLAGS)

client.o: client.cpp
	$(CPP) -c client.cpp $(CFLAGS)

server.o: server.cpp
	$(CPP) -c client.cpp $(CFLAGS)
	
remake: clean client server

clean:
	$(RM) client server *.o
