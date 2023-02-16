CPP := g++
CFLAGS := -g -Wall
LIBS := -lpthread
TARGET := main

default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CPP) -o $(TARGET) $(TARGET).o $(CFLAGS) $(LIBS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS) $(LIBS)

clean:
	$(RM) $(TARGET) *.o

remake: clean $(TARGET)
