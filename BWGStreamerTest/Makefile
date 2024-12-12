# Compiler and flags
CXX = g++
CXXFLAGS = -Wall  -std=c++17

# Include directories for OpenCV
OPENCV_CFLAGS = $(shell pkg-config --cflags opencv4)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)

# Output binary
TARGET = camera_pipeline

# Source files
SRC = main.cpp

# Build rules
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(OPENCV_CFLAGS) -o $@ $^ $(OPENCV_LIBS)

# Clean up build files
clean:
	rm -f $(TARGET)
