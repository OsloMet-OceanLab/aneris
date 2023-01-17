CPP := g++
CFLAGS := -g -Wall

default: all

udp: UDPserver.o UDPclient.o
	$(CPP) -o UDPserver UDPserver.o $(CFLAGS)
	$(CPP) -o UDPclient UDPclient.o $(CFLAGS)
	$(RM) *.o

UDPclient.o: UDPclient.cpp
	$(CPP) -c UDPclient.cpp $(CFLAGS)
	
UDPserver.o: UDPserver.cpp
	$(CPP) -c UDPserver.cpp $(CFLAGS)

tcp: TCPclient.o TCPserver.o
	$(CPP) -o TCPclient TCPclient.o $(CFLAGS) $(LIBS)
	$(CPP) -o TCPserver TCPserver.o $(CFLAGS) $(LIBS)
	$(RM) *.o
	
TCPclient.o: TCPclient.cpp
	$(CPP) -c TCPclient.cpp $(CFLAGS)
	
TCPserver.o: TCPserver.cpp
	$(CPP) -c TCPserver.cpp $(CFLAGS)

cleanudp:
	$(RM) UDPclient UDPserver

cleantcp:
	$(RM) TCPclient TCPserver

cleanall:
	$(RM) UDPclient UDPserver TCPclient TCPserver
