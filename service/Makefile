CPP := g++
CFLAGS := -g -Wall -O3
PYTHON_LIB := -I/usr/include/python3.9 -lpython3.9
THREAD_LIB := -lpthread
TARGET := aneris
INSTALL_DIR := /etc/$(TARGET)

default: $(TARGET)

install:
	mkdir $(INSTALL_DIR)
	cp $(TARGET) web_server.py $(INSTALL_DIR)
	cp -r web/ $(INSTALL_DIR)
	cp $(TARGET).service /etc/systemd/system/
	systemctl daemon-reload

$(TARGET): $(TARGET).o GPIO.o Logger.o Web_Server.o
	$(CPP) -o $(TARGET) $(TARGET).o GPIO.o Logger.o Web_Server.o $(CFLAGS) $(THREAD_LIB) $(PYTHON_LIB)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS) $(THREAD_LIB)
	
GPIO.o: gpio/GPIO.cpp
	$(CPP) -c gpio/GPIO.cpp $(CFLAGS)
	
Logger.o: Logger/Logger.cpp
	$(CPP) -c Logger/Logger.cpp $(CFLAGS)

Web_Server.o: Web_Server/Web_Server.cpp
	$(CPP) -c Web_Server/Web_Server.cpp $(CFLAGS) $(PYTHON_LIB)

clean:
	$(RM) $(TARGET) *.o
	
remake: clean $(TARGET)
