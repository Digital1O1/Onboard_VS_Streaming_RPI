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
CMAKE_SOURCE_DIR = "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build"

# Include any dependencies generated for this target.
include CMakeFiles/Homography.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Homography.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Homography.dir/flags.make

CMakeFiles/Homography.dir/main.cpp.o: CMakeFiles/Homography.dir/flags.make
CMakeFiles/Homography.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Homography.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Homography.dir/main.cpp.o -c "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/main.cpp"

CMakeFiles/Homography.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Homography.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/main.cpp" > CMakeFiles/Homography.dir/main.cpp.i

CMakeFiles/Homography.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Homography.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/main.cpp" -o CMakeFiles/Homography.dir/main.cpp.s

# Object files for target Homography
Homography_OBJECTS = \
"CMakeFiles/Homography.dir/main.cpp.o"

# External object files for target Homography
Homography_EXTERNAL_OBJECTS =

Homography: CMakeFiles/Homography.dir/main.cpp.o
Homography: CMakeFiles/Homography.dir/build.make
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_shape.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_stitching.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_superres.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_videostab.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_aruco.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_bgsegm.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_bioinspired.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_ccalib.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_datasets.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_dpm.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_face.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_freetype.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_fuzzy.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_hdf.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_line_descriptor.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_optflow.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_plot.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_reg.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_saliency.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_stereo.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_structured_light.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_surface_matching.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_text.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_ximgproc.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_xobjdetect.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_xphoto.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_video.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_viz.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_phase_unwrapping.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_rgbd.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_flann.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_ml.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_photo.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_videoio.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_imgcodecs.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so.3.2.0
Homography: /usr/lib/arm-linux-gnueabihf/libopencv_core.so.3.2.0
Homography: CMakeFiles/Homography.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Homography"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Homography.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Homography.dir/build: Homography

.PHONY : CMakeFiles/Homography.dir/build

CMakeFiles/Homography.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Homography.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Homography.dir/clean

CMakeFiles/Homography.dir/depend:
	cd "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build" "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Zoom Homography/build/CMakeFiles/Homography.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Homography.dir/depend

