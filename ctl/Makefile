CPP := g++
CFLAGS := -g -Wall
TARGET := aneris-ctl

default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CPP) -o $(TARGET) $(TARGET).o $(CFLAGS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)

clean:
	$(RM) $(TARGET) *.o

remake: clean $(TARGET)
