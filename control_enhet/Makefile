CPP := g++
CFLAGS := -g -Wall
TARGET := main

default: $(TARGET)

$(TARGET): $(TARGET).o GPIO.o
	$(CPP) -o $(TARGET) $(TARGET).o GPIO.o $(CFLAGS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
GPIO.o: ../gpio/C++/GPIO.cpp
	$(CPP) -c ../gpio/C++/GPIO.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
