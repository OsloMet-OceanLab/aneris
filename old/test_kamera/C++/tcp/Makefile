CPP := g++
CFLAGS := -g -Wall
LIBS := -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

default: tcp

tcp: tcp_client.o tcp_server.o
	$(CPP) -o tcp_server tcp_server.o $(CFLAGS) $(LIBS)
	$(CPP) -o tcp_client tcp_client.o $(CFLAGS) $(LIBS)
	$(RM) *.o

clean:
	$(RM) tcp_client tcp_server
