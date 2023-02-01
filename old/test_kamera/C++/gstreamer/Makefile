CPP := g++
CFLAGS := -g -Wall
GST := `pkg-config --cflags --libs gstreamer-1.0`
OPENCV := -lopencv_core -lopencv_videoio -lopencv_video -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

default: gst

udp: udp_serv.o
	$(CPP) -o udp_serv udp_serv.o $(CFLAGS) $(GST)
	$(RM) *.o

udp_serv.o: udp_serv.cpp
	$(CPP) -c udp_serv.cpp $(CFLAGS) $(GST)

gst: gst_serv.o
	$(CPP) -o gst_serv gst_serv.o $(CFLAGS) $(GST)
	$(RM) *.o

gst_serv.o: gst_serv.cpp
	$(CPP) -c gst_serv.cpp $(CFLAGS) $(GST)
	
gst_cli.o: gst_cli.cpp
	$(CPP) -c gst_cli.cpp $(CFLAGS) $(GST)

clean:
	$(RM) gst_serv gst_cli
