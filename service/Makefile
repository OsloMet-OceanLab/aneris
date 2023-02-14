CPP := g++
CFLAGS := -g -Wall
LIBS := -lpthread -I/usr/include/python3.9 -lpython3.9
TARGET := aneris

default: $(TARGET)

$(TARGET): $(TARGET).o GPIO.o Logger.o
	$(CPP) -o $(TARGET) $(TARGET).o GPIO.o Logger.o $(CFLAGS) $(LIBS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS) $(LIBS)
	
GPIO.o: gpio/GPIO.cpp
	$(CPP) -c gpio/GPIO.cpp $(CFLAGS)
	
Logger.o: Logger/Logger.cpp
	$(CPP) -c Logger/Logger.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
	
remake: clean $(TARGET)
