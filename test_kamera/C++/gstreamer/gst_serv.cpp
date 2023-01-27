#include <gst/gst.h>

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GstElement *pipeline, *src, *hls, *sink;

    pipeline = gst_pipeline_new("my-pipeline");
    src = gst_element_factory_make("v4l2src", "webcam-src");
    hls = gst_element_factory_make("hlssink", "hls-sink");
    sink = gst_element_factory_make("filesink", "file-sink");

    g_object_set(G_OBJECT(src), "device", "/dev/video0", NULL);
    g_object_set(G_OBJECT(hls), "location", "hls/stream.m3u8", NULL);
    g_object_set(G_OBJECT(hls), "playlist-location", "hls/playlist.m3u8", NULL);
    g_object_set(G_OBJECT(sink), "location", "hls/segment%05d.ts", NULL);
    g_object_set(G_OBJECT(hls), "max-files", 5, NULL);

    gst_bin_add_many(GST_BIN(pipeline), src, hls, sink, NULL);
    gst_element_link_many(src, hls, sink, NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    gst_element_get_state(pipeline, NULL, NULL, GST_CLOCK_TIME_NONE);

    return 0;
}

