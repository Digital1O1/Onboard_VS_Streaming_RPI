# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing "

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build"

# Include any dependencies generated for this target.
include CMakeFiles/libcamera_example.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libcamera_example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libcamera_example.dir/flags.make

CMakeFiles/libcamera_example.dir/main.cpp.o: CMakeFiles/libcamera_example.dir/flags.make
CMakeFiles/libcamera_example.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/libcamera_example.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcamera_example.dir/main.cpp.o -c "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /main.cpp"

CMakeFiles/libcamera_example.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcamera_example.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /main.cpp" > CMakeFiles/libcamera_example.dir/main.cpp.i

CMakeFiles/libcamera_example.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcamera_example.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /main.cpp" -o CMakeFiles/libcamera_example.dir/main.cpp.s

# Object files for target libcamera_example
libcamera_example_OBJECTS = \
"CMakeFiles/libcamera_example.dir/main.cpp.o"

# External object files for target libcamera_example
libcamera_example_EXTERNAL_OBJECTS =

libcamera_example: CMakeFiles/libcamera_example.dir/main.cpp.o
libcamera_example: CMakeFiles/libcamera_example.dir/build.make
libcamera_example: CMakeFiles/libcamera_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable libcamera_example"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libcamera_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libcamera_example.dir/build: libcamera_example

.PHONY : CMakeFiles/libcamera_example.dir/build

CMakeFiles/libcamera_example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libcamera_example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libcamera_example.dir/clean

CMakeFiles/libcamera_example.dir/depend:
	cd "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing " "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing " "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Testing /build/CMakeFiles/libcamera_example.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/libcamera_example.dir/depend

