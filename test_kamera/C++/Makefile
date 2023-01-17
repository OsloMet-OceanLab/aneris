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
	
udp: client.o server.o
	$(CPP) -o client client.o $(CFLAGS) $(LIBS)
	$(CPP) -o server server.o $(CFLAGS) $(LIBS)
	$(RM) *.o

client.o: client.cpp
	$(CPP) -c client.cpp $(CFLAGS)
	
server.o: server.cpp
	$(CPP) -c server.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)

udpclean:
	$(RM) client server
	
cleanall:
	$(RM) client server $(TARGET)
