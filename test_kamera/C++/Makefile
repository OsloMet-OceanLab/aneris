CPP := g++
CFLAGS := -Wall -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
TARGET := main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CPP) -o $(TARGET) $(TARGET).cpp $(CFLAGS)

clean:
	$(RM) $(TARGET)
