#include <wiringPi.h>
#include <iostream>
const int gpioPin = 17;

/*
    Keep in mind:
        - Phyiscal pintout is what's printed on the silk screen
    +-----+-----+---------+------+---Pi 4B---+------+---------+-----+-----+
    | BCM | wPi |   Name  | Mode | Physical  | Mode | Name    | wPi | BCM |
    +-----+-----+---------+------+----++----+------+---------+-----+-----+
    |     |     |    3.3v |      |  1 || 2  |      | 5v      |     |     |
    |  17 |   8 |   SDA.1 | ALT5 |  3 || 4  |      | 5V      |     |     |
    |  18 |   9 |   SCL.1 | ALT5 |  5 || 6  |      | Ground  |     |     |
    |  27 |   7 | GPIO. 27|  OUT |  7 || 8  | ALT5 | GPIO. 28|     |     |
    |  22 |   0 |   SDA.0 | ALT3 |  9 || 10 |      | GPIO. 30|  0  |  27 |
    |  23 |   1 |   SCL.0 | ALT3 | 11 || 12 | ALT5 | GPIO. 31|  1  |  28 |
    |  24 |   2 |   CE1   |  OUT | 13 || 14 |      | Ground  |     |     |
    |  25 |   3 |   CE0   |  OUT | 15 || 16 |      | GPIO. 21|  4  |  29 |
    |   4 |   4 |   SCLK  | ALT0 | 17 || 18 |      | GPIO. 22|  5  |  30 |
    |     |     |    3.3v |      | 19 || 20 |      | GPIO. 23|  6  |  31 |
    |  10 |  10 |    MOSI | ALT0 | 21 || 22 |      | Ground  |     |     |
    |   9 |   9 |    MISO | ALT0 | 23 || 24 |      | GPIO. 25|  2  |  26 |
    |  11 |  11 |    SCLK | ALT0 | 25 || 26 |      | GPIO. 8 |  3  |  23 |
    |     |     |      0v |      | 27 || 28 |      | GPIO. 7 |  7  |   4 |
    |   0 |  30 |    SDA0 | ALT0 | 29 || 30 |      | Ground  |     |     |
    |   1 |  31 |    SCL0 | ALT0 | 31 || 32 | ALT0 | GPIO. 1 |  8  |   1 |
    |     |     |   PWM0  | ALT0 | 33 || 34 |      | Ground  |     |     |
    |     |     |   PWM1  | ALT0 | 35 || 36 |      | GPIO. 2 |  9  |  2  |
    |     |     |    CTS0 | ALT0 | 37 || 38 | ALT0 | UART0_RTS| 10 |  3  |
    |     |     |    RTS0 | ALT0 | 39 || 40 |      | UART0_CTS| 12 | 26  |
    +-----+-----+---------+------+----++----+------+---------+-----+-----+
    | BCM | wPi |   Name  | Mode | Physical  | Mode | Name    | wPi | BCM |
    +-----+-----+---------+------+---Pi 4B---+------+---------+-----+-----+

*/

int main()
{
    // Setup WiringPi
    if (wiringPiSetup() == -1)
    {
        // Handle initialization error
        std::cerr << "Couldn't intialize WiringPi" << std::endl;
        return 1;
    }

    // Set the GPIO pin as OUTPUT
    pinMode(gpioPin, OUTPUT);

    // Example: Turn the GPIO pin ON for 1 second
    digitalWrite(gpioPin, HIGH);
    delay(1000); // 1 second delay
    digitalWrite(gpioPin, LOW);

    return 0;
}
