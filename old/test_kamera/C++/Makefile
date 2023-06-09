CPP := g++
CFLAGS := -g -Wall
LIBS := -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
TARGET := main

default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CPP) -o $(TARGET) $(TARGET).o $(CFLAGS) $(LIBS)
	$(RM) *.o
	
$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
udp: udp_client.o udp_server.o
	$(CPP) -o udp_server udp_server.o $(CFLAGS) $(LIBS)
	$(CPP) -o udp_client udp_client.o $(CFLAGS) $(LIBS)
	$(RM) *.o

udp_client.o: udp_client.cpp
	$(CPP) -c udp_client.cpp $(CFLAGS)
	
udp_server.o: udp_server.cpp
	$(CPP) -c udp_server.cpp $(CFLAGS)
	
tcp: tcp_client.o tcp_server.o
	$(CPP) -o tcp_server tcp_server.o $(CFLAGS) $(LIBS)
	$(CPP) -o tcp_client tcp_client.o $(CFLAGS) $(LIBS)
	$(RM) *.o

tcp_client.o: tcp_client.cpp
	$(CPP) -c tcp_client.cpp $(CFLAGS)
	
tcp_server.o: tcp_server.cpp
	$(CPP) -c tcp_server.cpp $(CFLAGS)
	
size: size_test.o
	$(CPP) -o size_test size_test.o $(CFLAGS) $(LIBS)

size_test.o: size_test.cpp
	$(CPP) -c size_test.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)

cleanudp:
	$(RM) udp_client udp_server
	
cleantcp:
	$(RM) tcp_client tcp_server
	
cleansize:
	$(RM) size_test
	
cleanall:
	$(RM) udp_client udp_server tcp_client tcp_server size_test $(TARGET)
