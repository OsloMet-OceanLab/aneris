CPP := g++
CFLAGS := -g -Wall
TARGET := main
LIBS := -lwiringPi

default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CPP) -o $(TARGET) $(TARGET).o $(CFLAGS) $(LIBS)
	$(RM) *.o
	
$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
PWM.o: PWM.cpp
	$(CPP) -c PWM.cpp $(CFLAGS)
	
clean:
	$(RM) $(TARGET)
