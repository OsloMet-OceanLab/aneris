CPP := g++
CFLAGS := -g -Wall
GST := `pkg-config --cflags --libs gstreamer-1.0`
OPENCV := -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

default: gst

gst: gst_serv.o gst_cli.o
	$(CPP) -o gst_serv gst_serv.o $(CFLAGS) $(GST)
	$(CPP) -o gst_cli gst_cli.o $(CFLAGS) $(GST)
	$(RM) *.o

gst_serv.o: gst_serv.cpp
	$(CPP) -c gst_serv.cpp $(CFLAGS) $(GST)
	
gst_cli.o: gst_cli.cpp
	$(CPP) -c gst_cli.cpp $(CFLAGS) $(GST)

clean:
	$(RM) gst_serv gst_cli
