#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#define HOST "0.0.0.0" // all interfaces
#define PORT 5000

int main(int argc, char **argv)
{
	gst_init(&argc, &argv);
	
	GstElement *pipeline, *src, *filter, *sink;
	pipeline = gst_pipeline_new("pipeline");
	src = gst_element_factory_make("v4l2src", "src");
	filter = gst_element_factory_make("capsfilter", "filter");
	sink = gst_element_factory_make("udpsink", "sink");
	
	g_object_set(G_OBJECT(filter), "caps",
					gst_caps_new_simple("video/x-raw",
										"width", G_TYPE_INT, 1280,
										"height", G_TYPE_INT, 720,
										"framerate", GST_TYPE_FRACTION,
										30, 1, NULL),
										NULL);
										
	g_object_set(G_OBJECT(sink), "host", HOST, "port", PORT, NULL);
	gst_bin_add_many(GST_BIN(pipeline), src, filter, sink, NULL);
	gst_element_link_many(src, filter, sink, NULL);
	
	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	
	return 0;
}
