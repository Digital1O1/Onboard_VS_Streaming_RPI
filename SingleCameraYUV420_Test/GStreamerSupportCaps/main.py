import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst

Gst.init(None)

# Create the libcamerasrc element
src = Gst.ElementFactory.make("libcamerasrc")

# Get the pad and query its capabilities
pad = src.get_static_pad("src")
caps = pad.query_caps(None)

# Print all supported caps
print("Supported caps:")
for i in range(caps.get_size()):
    structure = caps.get_structure(i)
    print(structure.to_string())
