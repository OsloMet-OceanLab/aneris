CPP := g++
CFLAGS := -O3 -Wpedantic -Wall -Wextra -Wconversion -Wshadow -Werror
#PYTHON_LIB := -I/usr/include/python3.9 -lpython3.9
PYTHON_LIB := $(shell python3-config --includes) -lpython3.9
THREAD_LIB := -lpthread

#PYBIND_LIB := $(shell python3 -m pybind11 --includes)
#CPYTHON_EXTENSION := $(shell python3-config --extension-suffix)

TARGET := aneris
INSTALL_DIR := /etc/$(TARGET)/

default: $(TARGET) $(TARGET)-ctl

$(TARGET): $(TARGET).o GPIO.o Logger.o Web_Server.o
	$(CPP) -o $@ $? $(CFLAGS) $(THREAD_LIB) $(PYTHON_LIB)
	$(RM) *.o

$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $< $(CFLAGS) $(THREAD_LIB)
	
GPIO.o: gpio/GPIO.cpp
	$(CPP) -c $< $(CFLAGS)
	
Logger.o: Logger/Logger.cpp
	$(CPP) -c $< $(CFLAGS)

Web_Server.o: Web_Server/Web_Server.cpp
	$(CPP) -c $< $(CFLAGS) $(PYTHON_LIB)

Cpython: bit_converter/bit_converter.cpp
	$(CPP) -shared -O3 -Wall -fPIC $(PYBIND_LIB) $< -o bit_converter$(CPYTHON_EXTENSION)

$(TARGET)-ctl: $(TARGET)-ctl.cpp
	$(CPP) -o $@ $< $(CFLAGS)

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
