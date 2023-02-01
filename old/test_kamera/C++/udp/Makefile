CPP := g++
CFLAGS := -g -Wall
LIBS := -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
TARGET := main

default: udp

udp: udp_client.o udp_server.o
	$(CPP) -o udp_server udp_server.o $(CFLAGS) $(LIBS)
	$(CPP) -o udp_client udp_client.o $(CFLAGS) $(LIBS)
	$(RM) *.o

udp_client.o: udp_client.cpp
	$(CPP) -c udp_client.cpp $(CFLAGS)
	
udp_server.o: udp_server.cpp
	$(CPP) -c udp_server.cpp $(CFLAGS)
	
clean:
	$(RM) udp_server udp_client

