#include <iostream>
#include <thread>
#include <cstdlib>

void runBashScript()
{
    // Launch the Bash script using the system() call
    int result = std::system("/home/pi/Onboard_VS_Streaming_RPI/GStreamerPipelines/BWDualGStreamerPipelines.sh");
    if (result != 0)
    {
        std::cerr << "Error running the Bash script" << std::endl;
    }
    else
    {
        printf("Running script\r\n");
    }
}

void controlGPIO()
{
    // Example GPIO control logic
    while (true)
    {
        // Code to manipulate GPIO pins goes here
        std::cout << "Controlling GPIO..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    // Start the Bash script in a separate thread
    std::thread scriptThread(runBashScript);

    // Run GPIO control in the main thread
    controlGPIO();

    // Wait for the Bash script thread to finish (if needed)
    scriptThread.join();

    return 0;
}
