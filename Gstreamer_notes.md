# [Basic tutorial 2: GStreamer concepts](https://gstreamer.freedesktop.org/documentation/tutorials/basic/concepts.html?gi-language=c)

## Goal
- Learn what a `GStreamer element` is
- How to connect elements to each other
- Customize element's behavior 
- Learn how to watch bus for `error conditions` and `extract info from GStreamer messages`

## Code can be found here : /home/pi/Desktop/Onboard_VS_Streaming_RPI/GStreamer/GStreamer Tutorial Folder/Basic tutorial 2: GStreamer concepts

## Walkthrough 

### Elements
- GStreamer's basic construction blocks
- Process the data as it flows `downstream` from the source elements (data producers) --> `sink elements` (data consumers) passing through filter elements 
  - Example pipeline listed below
  
    ![alt text](<GStreamer/GStreamer Tutorial Folder/Screenshots/figure-1.png>)

### Element creation
```c
  /* Create the elements */
  source = gst_element_factory_make ("videotestsrc", "source");
  sink = gst_element_factory_make ("autovideosink", "sink");
```
- Summarizing the code above
  - New elements made with `gst_element_factory_make ("videotestsrc", "source")`
    - First parameter : The `type` of element we want to create 
      - videotestsrc
    - Second parameter : The `name` we want to give to that particular instance/element
      - source
      - If `NULL` is passed for the name 
        - Gstreamer will provide unique name for you

  - In this tutorial two elements were made
    - videotestsrc
      - The `source` element
        - Aka, it `produces data`
      - Creates test video pattern
      - Useful for `debugging`
      - Not typically found in real applications
    - autovideosink
      - The `sink` element
        - Aka, it `consumes data`
      - Displays images it recieves in window
      - There's numerous video sinks 
      - `autovideosink` automatically selects and instantiates the best one 
  - No filter elements which is why pipeline looks like this

    ![alt text](<GStreamer/GStreamer Tutorial Folder/Screenshots/basic-concepts-pipeline.png>)


### Pipeline creation

```c
  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");
```

- All elements in GStreamer must be contained `inside a pipeline` before they can be used
  - Because it'll take care of some `clocking` and `messaging function`

```c
  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
  if (gst_element_link (source, sink) != TRUE) 
  {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }
```

- A `pipeline` is a particular type of `bin`
  - bin : element that can contain another `element`
  - Allowing them to be managed as a group
  - All methods that apply to `bins` also applies to `pipelines`

- In this case we call `gst_bin_add_many()` to add elements to the pipeline
  - Ignore the cast
  - This function accepts a list of elements to be added 
    - Ending with a `NULL`
  - Individual elements can be added with `gst_bin_add()`
    - In this example `sink` and `source` are the bins
  - All these elements aren't linked together though
    - To link them, we must use `gst_element_link()`
      - First parameter : The source 
      - Second parameter : The destination
      - `Order does count` since the links must be established following the data flow
        - From `source elements` to `sink elements`
- Remember  
  - Only elements in the same `bin` can be `linked together`
  - Must add them to pipeline before linking them

## Properties 
- Most GStreamer elements have customizable properties 
  - Named attributes that can be modified to change the element's behavior 
  - Or ask to find out about the element's internal states
- Properties are
  - Read : `g_object_get()`
  - Written : ` g_object_set()`
    - Accepts `NULL terminated list` of the following properties that can be changed in one go
      - Property-name
      - Property-value pairs
      - Hence the property handling method having the `_g` prefix
      
## Error checking
- Calling `gst_element_set_state()`
  - Sets state of element
  - Function can return `GST_STATE_CHANGE_ASYNC`
    - Element will perform remainder of the state change `asynchronously` in another thread
  - Applicationst that use this function to wait for the state change wait for either a 
    - `GST_MESSAGE_ASYNC_DONE`
    - `GST_MESSAGE_STATE_CHANGE`
      ```c
        /* Start playing */
        ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
        if (ret == GST_STATE_CHANGE_FAILURE) {
          g_printerr ("Unable to set the pipeline to the playing state.\n");
          gst_object_unref (pipeline);
          return -1;
        }
      ```
- Calling `gst_element_set_state()`
  - It's return value is for errors

- `gst_bus_timed_pop_filtered() `
  - waits for execution to end
  - returns with `GstMessage`

- `gst_bus_timed_pop_filtered()`
  - asked to return when GStreamer either hits an `error condition` or an `EOS`
    - EOS = End of Stream
    - Need to check which one happened 
    - Then print message to screen
- `GstMessage`
  - Versatile 
  - Can deliver almost any kind of information
  - GStreamer does provide parsing functions for each kind of message
  - Once we know the message has an error when using `GST_MESSAGE_TYPE`
    - Can use `gst_message_parse_error()`
      - Returns 
        - Glib `Gerror` strcuture
        - A string useful for debugging 

```cpp
 /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);

  // Waits for execution and returns GstMessage
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Parse message */
  if (msg != NULL) 
  {
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE (msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error (msg, &err, &debug_info);
        g_printerr ("Error received from element %s: %s\n",
            GST_OBJECT_NAME (msg->src), err->message);
        g_printerr ("Debugging information: %s\n",
            debug_info ? debug_info : "none");
        g_clear_error (&err);
        g_free (debug_info);
        break;
      case GST_MESSAGE_EOS:
        g_print ("End-Of-Stream reached.\n");
        break;
      default:
        /* We should not reach here because we only asked for ERRORs and EOS */
        g_printerr ("Unexpected message received.\n");
        break;
    }
    gst_message_unref (msg);
  }

```

## GStreamer bus
- This object is responsible for delivering the `GstMessages` generated from the elements to the application thread
- Messages can be extracted from the bus synchronously with `gst_bus_timed_pop_filter()` and it's 'siblings'
  -   Or asynchronously using `signals`
- Your application should always keep an eye on the bus for errors and other playback-related issues