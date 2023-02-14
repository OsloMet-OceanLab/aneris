CPP := g++
CFLAGS := -g -Wall
LIBS := -lpthread -I/usr/include/python3.9 -lpython3.9
TARGET := aneris

default: $(TARGET)

$(TARGET): $(TARGET).o GPIO.o Logger.o Web_Server.o
	$(CPP) -o $(TARGET) $(TARGET).o GPIO.o Logger.o Web_Server.o $(CFLAGS) $(LIBS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
GPIO.o: gpio/GPIO.cpp
	$(CPP) -c gpio/GPIO.cpp $(CFLAGS)
	
Logger.o: Logger/Logger.cpp
	$(CPP) -c Logger/Logger.cpp $(CFLAGS)

Web_Server.o: Web_Server/Web_Server.cpp
	$(CPP) -c Web_Server/Web_Server.cpp $(CFLAGS) $(LIBS)

clean:
	$(RM) $(TARGET) *.o
	
remake: clean $(TARGET)
