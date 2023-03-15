CPP := g++
CFLAGS := -O3 -Wpedantic -Wall -Wextra -Wconversion -Wshadow -Werror
#PYTHON_LIB := -I/usr/include/python3.9 -lpython3.9
PYTHON_LIB := $(shell python3-config --includes) -lpython3.9
THREAD_LIB := -lpthread

#PYBIND_LIB := $(shell python3 -m pybind11 --includes)
#CPYTHON_EXTENSION := $(shell python3-config --extension-suffix)

TARGET := aneris
INSTALL_DIR := /etc/$(TARGET)/

default: $(TARGET)

$(TARGET): $(TARGET).o GPIO.o Logger.o Web_Server.o ctl
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

Cpython: bit_converter/bit_converter.cpp
	$(CPP) -shared -O3 -Wall -fPIC $(PYBIND_LIB) bit_converter/bit_converter.cpp -o bit_converter$(CPYTHON_EXTENSION)

ctl: $(TARGET)-ctl.cpp
	$(CPP) -o $(TARGET)-ctl $(TARGET)-ctl.cpp $(CFLAGS)

install:
	mkdir $(INSTALL_DIR)
	cp $(TARGET) web_server.py $(INSTALL_DIR)
	cp -r web/ $(INSTALL_DIR)
	cp $(TARGET).service /etc/systemd/system/
	cp $(TARGET)-ctl /usr/bin/
	systemctl daemon-reload

clean:
	$(RM) $(TARGET) $(TARGET)-ctl *.o
	
remake: clean $(TARGET)
