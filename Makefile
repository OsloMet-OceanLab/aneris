CPP := g++
CFLAGS := -O3 -Wpedantic -Wall -Wextra -Wconversion -Wshadow -Werror
PYTHON_LIB := $(shell python3-config --includes) -lpython3.9
THREAD_LIB := -lpthread

TARGET := aneris
SRC_DIR := ./src
INSTALL_DIR := /etc/$(TARGET)/

default: dependencies $(TARGET) $(TARGET)-ctl

dependencies:
	apt install python3.9-dev g++ python3-picamera
	pip3 install pybind11 opencv-python

$(TARGET): $(TARGET).o GPIO.o Logger.o Web_Server.o
	$(CPP) -o $@ $? $(CFLAGS) $(THREAD_LIB) $(PYTHON_LIB)

$(TARGET).o: $(SRC_DIR)/$(TARGET).cpp
	$(CPP) -c $< $(CFLAGS) $(THREAD_LIB)
	
GPIO.o: $(SRC_DIR)/GPIO.cpp
	$(CPP) -c $< $(CFLAGS)
	
Logger.o: $(SRC_DIR)/Logger.cpp
	$(CPP) -c $< $(CFLAGS)

Web_Server.o: $(SRC_DIR)/Web_Server.cpp
	$(CPP) -c $< $(CFLAGS) $(PYTHON_LIB)

$(TARGET)-ctl: $(SRC_DIR)/$(TARGET)-ctl.cpp
	$(CPP) -o $@ $< $(CFLAGS)

install:
	mkdir -p $(INSTALL_DIR)
	mkdir -p /var/log/aneris/
	cp $(TARGET) $(SRC_DIR)/web_server.py $(INSTALL_DIR)
	cp -r www/ $(INSTALL_DIR)
	cp $(TARGET).service /etc/systemd/system/
	cp $(TARGET)-ctl /usr/bin/
	systemctl daemon-reload
	systemctl enable aneris
	systemctl start aneris

clean:
	$(RM) $(TARGET) $(TARGET)-ctl *.o

remake: clean $(TARGET)
