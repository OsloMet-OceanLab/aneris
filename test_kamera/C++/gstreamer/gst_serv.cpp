#include <gstreamer-1.0/gst/gst.h>
#include <opencv2/opencv.hpp>

int main()
{
	// Initialize Gstreamer pipeline
	GstElement *pipeline;
	pipeline = gst_pipeline_new("webcam-server");

	// Create elements for pipeline
	GstElement *source = gst_element_factory_make("v4l2src", "webcam-source");
	GstElement *convert = gst_element_factory_make("videoconvert", "convert");
	GstElement *encoder = gst_element_factory_make("x264enc", "encoder");
	GstElement *payloader = gst_element_factory_make("rtph264pay", "payloader");
	GstElement *sink = gst_element_factory_make("udpsink", "udp-sink");

	// Set properties for elements
	g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);
	g_object_set(G_OBJECT(encoder), "bitrate", 2000, NULL);
	g_object_set(G_OBJECT(payloader), "config-interval", 1, NULL);
	g_object_set(G_OBJECT(sink), "host", "localhost", NULL);
	g_object_set(G_OBJECT(sink), "port", 5000, NULL);

	// Add elements to pipeline
	gst_bin_add_many(GST_BIN(pipeline), source, convert, encoder, payloader, sink, NULL);

	// Link elements together
	gst_element_link_many(source, convert, encoder, payloader, sink, NULL);

	// Start pipeline
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	// OpenCV code to display live feed
	cv::VideoCapture cap(0);
	cv::Mat frame;
	while (true) {
	cap >> frame;
	cv::imshow("Webcam Live Feed", frame);
	if (cv::waitKey(30) >= 0) break;
	}

	// Stop pipeline
	gst_element_set_state(pipeline, GST_STATE_NULL);

	return 0;
}

