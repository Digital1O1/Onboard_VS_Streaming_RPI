import select
import sys
import tty
import termios
import os
import time
import RPi.GPIO as GPIO

# GPIO pin definitions
GREEN_LASER_PIN = 25
EXCITATION_PIN = 7  # Active LOW

def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setup(EXCITATION_PIN, GPIO.OUT)
GPIO.setup(GREEN_LASER_PIN, GPIO.OUT)

# Initial states
GPIO.output(EXCITATION_PIN, GPIO.HIGH)     # OFF (because it's active LOW)
GPIO.output(GREEN_LASER_PIN, GPIO.LOW)     # Laser OFF
laser_on = False

# Terminal setup
old_settings = termios.tcgetattr(sys.stdin)
try:
    tty.setcbreak(sys.stdin.fileno())
    print("Press SPACE to toggle laser or ESC to exit.")
    while True:
        if isData():
            c = sys.stdin.read(1)
            if c == '\x1b':  # ESC key
                print("Escape pressed. Exiting...")
                break
            elif c == ' ':
                laser_on = not laser_on
                GPIO.output(GREEN_LASER_PIN, GPIO.HIGH if laser_on else GPIO.LOW)
                GPIO.output(EXCITATION_PIN , GPIO.LOW if laser_on else GPIO.HIGH)
                print(f"Spacebar pressed - Laser {'ON' if laser_on else 'OFF'}")
            else:
                print(f"Read: {c}")

finally:
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
    GPIO.output(GREEN_LASER_PIN, GPIO.LOW)  # Turn off laser before exiting
    GPIO.cleanup()
    print("GPIO cleaned up. Program terminated.")

