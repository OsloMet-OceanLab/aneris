#include <gst/gst.h>

int main(int argc, char *argv[]) {
  gst_init(&argc, &argv);

  GstElement *pipeline, *source, *encoder, *payloader, *sink;

  pipeline = gst_pipeline_new("webcam-streamer");
  source = gst_element_factory_make("v4l2src", "webcam-source");
  encoder = gst_element_factory_make("vp8enc", "encoder");
  payloader = gst_element_factory_make("rtpvp8pay", "payloader");
  sink = gst_element_factory_make("udpsink", "udp-sink");

  g_object_set(G_OBJECT(sink), "host", "127.0.0.1", NULL);
  g_object_set(G_OBJECT(sink), "port", 5000, NULL);

  gst_bin_add_many(GST_BIN(pipeline), source, encoder, payloader, sink, NULL);
  gst_element_link_many(source, encoder, payloader, sink, NULL);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);

  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}

