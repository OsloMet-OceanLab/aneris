CPP := g++
CFLAGS := -g -Wall
LIBS := -lpthread
TARGET := aneris

default: $(TARGET)

$(TARGET): $(TARGET).o GPIO.o Logger.o
	$(CPP) -o $(TARGET) $(TARGET).o GPIO.o Logger.o $(CFLAGS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
GPIO.o: gpio/GPIO.cpp
	$(CPP) -c gpio/GPIO.cpp $(CFLAGS)
	
Logger.o: Logger/Logger.cpp
	$(CPP) -c Logger/Logger.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
	
remake: clean $(TARGET)
