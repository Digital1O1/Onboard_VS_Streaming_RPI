#include <X11/Xlib.h>
#include <iostream>

// g++ -o window_position main.cpp -lX11

int main()
{
    printf("Program started\n");
    Display *display = XOpenDisplay(NULL);
    if (!display)
    {
        std::cerr << "Error: Unable to open X display" << std::endl;
        return 1;
    }
    else
    {
        printf("--------------------- Display created ---------------------\n\nMove the window and release the mouse button click on it to print its location\n\n");
    }

    int squareWidth = 640;
    int squareHeight = 480;

    int screen = DefaultScreen(display);

    // Create a new window
    Window win = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, squareWidth, squareHeight, 0, BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, win, ButtonReleaseMask | ExposureMask);

    XMapWindow(display, win);

    XEvent event;

    while (true)
    {
        XNextEvent(display, &event);
        if (event.type == ButtonRelease)
        {
            int x, y;
            Window child;
            XTranslateCoordinates(display, win, DefaultRootWindow(display), 0, 0, &x, &y, &child);
            std::cout << "Window Location: " << x << ", " << y << std::endl;
        }
        else if (event.type == Expose)
        {
            // Redraw the window
            XClearWindow(display, win);
            XDrawRectangle(display, win, DefaultGC(display, screen), 0, 0, squareWidth, squareHeight);
            XFlush(display);
        }
    }

    XCloseDisplay(display);

    return 0;
}
