CPP := g++
CFLAGS := -O3 -Wpedantic -Wall -Wextra -Wconversion -Wshadow -Werror
TARGET := aneris-ctl

default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CPP) -o $(TARGET).out $(TARGET).o $(CFLAGS)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)

clean:
	$(RM) $(TARGET) *.o

remake: clean $(TARGET)
