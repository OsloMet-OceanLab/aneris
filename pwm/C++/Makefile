CPP := g++
CFLAGS := -g -Wall
TARGET := main

default: $(TARGET)

$(TARGET): $(TARGET).o PWM.o
	$(CPP) -o $(TARGET).o PWM.o $(CFLAGS)
	$(RM) *.o
	
$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)
	
PWM.o: PWM.cpp
	$(CPP) -c PWM.cpp $(CFLAGS)
	
clean:
	$(RM) $(TARGET)
