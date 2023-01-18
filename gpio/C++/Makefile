CPP := g++
CFLAGS := -g -Wall
TARGET := gpio

default: $(TARGET)

$(TARGET): main.o GPIO.o
	$(CPP) -o $(TARGET) main.o GPIO.o $(CFLAGS)
	
args: argmain.o GPIO.o
	$(CPP) -o args argmain.o GPIO.o $(CFLAGS)
	
main.o: main.cpp GPIO.h
	$(CPP) -c main.cpp $(CFLAGS)
	
argmain.o: argmain.cpp
	$(CPP) -c argmain.cpp $(CFLAGS)

GPIO.o: GPIO.cpp GPIO.h
	$(CPP) -c GPIO.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET) args *.o
	
