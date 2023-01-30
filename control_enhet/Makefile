CPP := g++
CFLAGS := -g -Wall
LIBS := -lpthread
TARGET := main

default: $(TARGET)

$(TARGET): $(TARGET).o GPIO.o Logger.o
	$(CPP) -o $(TARGET) $(TARGET).o GPIO.o Logger.o $(CFLAGS) $(LIBS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
GPIO.o: ../gpio/C++/GPIO.cpp
	$(CPP) -c ../gpio/C++/GPIO.cpp $(CFLAGS)
	
Logger.o: Logger/Logger.cpp
	$(CPP) -c Logger/Logger.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
