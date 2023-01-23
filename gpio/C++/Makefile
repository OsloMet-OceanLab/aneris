CPP := g++
CFLAGS := -g -Wall
TARGET := gpio

default: $(TARGET)

$(TARGET): main.o GPIO.o
	$(CPP) -o $(TARGET) main.o GPIO.o $(CFLAGS)

read: read.o GPIO.o
	$(CPP) -o read read.o GPIO.o $(CFLAGS)

args: argmain.o GPIO.o
	$(CPP) -o args argmain.o GPIO.o $(CFLAGS)

read.o: read.cpp
	$(CPP) -c read.cpp $(CFLAGS)

main.o: main.cpp GPIO.hpp
	$(CPP) -c main.cpp $(CFLAGS)
	
argmain.o: argmain.cpp
	$(CPP) -c argmain.cpp $(CFLAGS)

GPIO.o: GPIO.cpp GPIO.hpp
	$(CPP) -c GPIO.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET) args read *.o
	
