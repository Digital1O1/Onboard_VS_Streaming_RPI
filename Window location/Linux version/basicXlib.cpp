#include <X11/Xlib.h>
#include <iostream>

int main()
{
    Display *display = XOpenDisplay(NULL);
    if (!display)
    {
        std::cerr << "Error: Unable to open X display" << std::endl;
        return 1;
    }

    int screen_num = DefaultScreen(display);
    Window root_window = RootWindow(display, screen_num);

    int window_width = 640;
    int window_height = 480;

    Window window = XCreateSimpleWindow(display, root_window, 0, 0, window_width, window_height, 0,
                                        BlackPixel(display, screen_num), WhitePixel(display, screen_num));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (true)
    {
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == Expose)
        {
            // Handle expose event, such as redrawing the window content
            // For simplicity, we won't do anything here
        }
        else if (event.type == KeyPress)
        {
            // Handle key press event, such as closing the window
            break;
        }
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
