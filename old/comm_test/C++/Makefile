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
	$(CPP) -o TCPclient TCPclient.o $(CFLAGS)
	$(CPP) -o TCPserver TCPserver.o $(CFLAGS)
	$(RM) *.o
	
TCPclient.o: TCPclient.cpp
	$(CPP) -c TCPclient.cpp $(CFLAGS)
	
TCPserver.o: TCPserver.cpp
	$(CPP) -c TCPserver.cpp $(CFLAGS)
	
pipe: PIPEserv.o PIPEcli.o
	$(CPP) -o PIPEserv PIPEserv.o $(CFLAGS)
	$(CPP) -o PIPEcli PIPEcli.o $(CFLAGS)
	
PIPEserv.o: PIPEserv.cpp
	$(CPP) -c PIPEserv.cpp $(CFLAGS)

PIPEcli.o: PIPEcli.cpp
	$(CPP) -c PIPEcli.cpp $(CFLAGS)

cleanudp:
	$(RM) UDPclient UDPserver

cleantcp:
	$(RM) TCPclient TCPserver
	
cleanpipe:
	$(RM) PIPEserv PIPEcli

cleanall:
	$(RM) UDPclient UDPserver TCPclient TCPserver PIPEserv PIPEcli
