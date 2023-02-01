#include <iostream>
#include <gst/gst.h>
#include <glib-unix.h>

#define HOST	"127.0.0.1"
#define PORT	5000

int main(gint argc, gchar **argv)
{
	GstElement *pipeline, *videosrc, *conv, *enc, *pay, *udp;
	GMainLoop *loop;
	
	gst_init(&argc, &argv);
	loop = g_main_loop_new(NULL, false);
	
	pipeline = gst_pipeline_new("pipeline");
	videosrc = gst_element_factory_make("videotestsrc", "source");
	conv = gst_element_factory_make("videoconvert", "conv");
	enc = gst_element_factory_make("x264enc", "enc");
	pay = gst_element_factory_make("rtph264pay", "pay");
	g_object_set(G_OBJECT(pay), "config-interval", 1, NULL);
	g_object_set(G_OBJECT(udp), "host", HOST, NULL);
	g_object_set(G_OBJECT(udp), "port", PORT, NULL);
	
	gst_bin_add_many(GST_BIN(pipeline), videosrc, conv, enc, pay, udp, NULL);
	if(!gst_element_link_many(videosrc, conv, enc, pay, udp, NULL)) return -1;
	
	// play
	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	g_main_loop_run(loop);
	
	// cleanup
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(pipeline));
	g_main_loop_unref(loop);
	
	return 0;
}
