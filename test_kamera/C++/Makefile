CPP := g++
CFLAGS := -g -Wall
LIBS := -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
TARGET := main

default: $(TARGET)

$(TARGET): $(TARGET).o
	$(CPP) -o $(TARGET) $(TARGET).o $(CFLAGS) $(LIBS)
	
$(TARGET).o: $(TARGET).cpp
	$(CPP) -c $(TARGET).cpp $(CFLAGS)

clean:
	$(RM) $(TARGET) *.o
