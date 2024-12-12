# Define variables
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -O2
TARGET = gpio_script_control
SRCS = main.cpp

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up generated files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
