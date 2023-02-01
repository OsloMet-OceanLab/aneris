import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst

Gst.init(None)

# Create pipeline
pipeline = Gst.Pipeline.new("webcam-live-feed")

# Create elements
source = Gst.ElementFactory.make("v4l2src", "webcam-source")
sink = Gst.ElementFactory.make("xvimagesink", "video-output")

# Add elements to pipeline
pipeline.add(source)
pipeline.add(sink)

# Link elements
source.link(sink)

# Start pipeline
pipeline.set_state(Gst.State.PLAYING)

# Wait for pipeline to finish
bus = pipeline.get_bus()
bus.poll(Gst.MessageType.EOS, Gst.CLOCK_TIME_NONE)

# Clean up
pipeline.set_state(Gst.State.NULL)

