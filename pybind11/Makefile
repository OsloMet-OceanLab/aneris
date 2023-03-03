CPP := g++
FLAGS := -Wall -shared -O3
PYBIND := -fPIC $(python3 -m pybind11 --includes) 
TARGET := header
ENDTARGET := $(TARGET)$(python3-config --extension-suffix)
LIBS := -I /home/giorgio/.local/lib/python3.10/site-packages/pybind11/include/
PYTHON_LIB := -I/usr/include/python3.10 -lpython3.10

default: $(TARGET)

$(TARGET):
	$(CPP) $(FLAGS) $(PYBIND) $(TARGET).h -o $(ENDTARGET) $(LIBS) $(PYTHON_LIB)

clean:
	$(RM) $(TARGET)

