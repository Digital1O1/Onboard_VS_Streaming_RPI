#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <iomanip> // for std::put_time

// Function to read CPU statistics from /proc/stat
double readCpuUsage()
{
    std::ifstream file("/proc/stat");
    std::string line;

    // Read the first line of /proc/stat which contains total CPU usage
    std::getline(file, line);

    // Extract CPU usage information from the line
    std::istringstream iss(line);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    // Calculate total idle time and total time
    double totalTime = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    double totalIdle = idle + iowait;

    // Calculate CPU usage percentage
    double cpuUsage = ((totalTime - totalIdle) / totalTime) * 100.0;

    return cpuUsage;
}

int main()
{
    // Open a file stream to write CPU usage values to a CSV file
    std::ofstream outputFile("cpu_usage.csv");
    outputFile << "Time,CPU Usage (%)\n"; // Header for CSV file

    // Get CPU usage periodically
    while (true)
    {
        double cpuUsage = readCpuUsage();
        // Get current timestamp
        std::time_t now = std::time(nullptr);
        // Convert now to string form with desired format
        std::tm *timestamp = std::localtime(&now);
        char buffer[80];
        std::strftime(buffer, 80, "%a %b %d %T %Y", timestamp);
        // Write timestamp and CPU usage to CSV file on a single line
        outputFile << buffer << "," << std::fixed << std::setprecision(5) << cpuUsage << std::endl;

        // Sleep for 1 second
        sleep(1);
    }

    // Close the file stream
    outputFile.close();

    return 0;
}
