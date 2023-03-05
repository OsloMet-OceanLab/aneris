CPP := g++
CFLAGS := -Wall -shared -O3 -fPIC
PYBIND := $(shell python3 -m pybind11 --includes)
TARGET := bit_extension
EXTENSION := $(shell python3-config --extension-suffix)

default: $(TARGET)

$(TARGET):
        $(CPP) $(CFLAGS) $(PYBIND) $(TARGET).cpp -o $(TARGET)$(EXTENSION)

clean:
        $(RM) $(TARGET)$(EXTENSION)

remake: clean $(TARGET)
