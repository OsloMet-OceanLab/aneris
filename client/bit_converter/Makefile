CPP := g++
CFLAGS := -shared -O3 -Wall -fPIC
PYBIND := $(shell python3 -m pybind11 --includes)
TARGET := bit_converter
EXTENSION := $(shell python3-config --extension-suffix)

default: $(TARGET)

$(TARGET):
	$(CPP) $(CFLAGS) -c $(TARGET).cpp

cpython:
	$(CPP) $(CFLAGS) $(PYBIND) $(TARGET).cpp -o $(TARGET)$(EXTENSION) -DCPYTHON

clean:
	$(RM) $(TARGET)$(EXTENSION)

remake: clean $(TARGET)
