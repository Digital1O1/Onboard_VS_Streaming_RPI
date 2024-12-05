# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall `pkg-config --cflags opencv4 gstreamer-1.0 gstreamer-app-1.0`
LDFLAGS = `pkg-config --libs opencv4 gstreamer-1.0 gstreamer-app-1.0`

# Source files and output
SRC = main.cpp
TARGET = SingleStream

# Build rules
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)
