# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ziyad/Documents/Portfolio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ziyad/Documents/Portfolio

# Include any dependencies generated for this target.
include examples/gaussianBlurring/CMakeFiles/Gaussian.dir/depend.make

# Include the progress variables for this target.
include examples/gaussianBlurring/CMakeFiles/Gaussian.dir/progress.make

# Include the compile flags for this target's objects.
include examples/gaussianBlurring/CMakeFiles/Gaussian.dir/flags.make

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/flags.make
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o: examples/gaussianBlurring/source/Gaussian.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o -c /home/ziyad/Documents/Portfolio/examples/gaussianBlurring/source/Gaussian.cpp

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Gaussian.dir/source/Gaussian.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/examples/gaussianBlurring/source/Gaussian.cpp > CMakeFiles/Gaussian.dir/source/Gaussian.cpp.i

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Gaussian.dir/source/Gaussian.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/examples/gaussianBlurring/source/Gaussian.cpp -o CMakeFiles/Gaussian.dir/source/Gaussian.cpp.s

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.requires:

.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.requires

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.provides: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.requires
	$(MAKE) -f examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build.make examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.provides.build
.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.provides

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.provides.build: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o


examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/flags.make
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o: dependencies/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o -c /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui.cpp

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui.cpp > CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.i

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui.cpp -o CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.s

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.requires:

.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.requires

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.provides: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
	$(MAKE) -f examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build.make examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build
.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.provides

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o


examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/flags.make
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: dependencies/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o -c /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_draw.cpp

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_draw.cpp > CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.i

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_draw.cpp -o CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.s

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires:

.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
	$(MAKE) -f examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build.make examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build
.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o


examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/flags.make
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: dependencies/imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o -c /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_demo.cpp

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_demo.cpp > CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.i

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_demo.cpp -o CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.s

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires:

.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires
	$(MAKE) -f examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build.make examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build
.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o


# Object files for target Gaussian
Gaussian_OBJECTS = \
"CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o" \
"CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o" \
"CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o" \
"CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o"

# External object files for target Gaussian
Gaussian_EXTERNAL_OBJECTS =

examples/gaussianBlurring/Scene: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o
examples/gaussianBlurring/Scene: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o
examples/gaussianBlurring/Scene: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o
examples/gaussianBlurring/Scene: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o
examples/gaussianBlurring/Scene: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build.make
examples/gaussianBlurring/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/gaussianBlurring/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/gaussianBlurring/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/gaussianBlurring/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/gaussianBlurring/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/gaussianBlurring/Scene: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Scene"
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Gaussian.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build: examples/gaussianBlurring/Scene

.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/build

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/requires: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/source/Gaussian.cpp.o.requires
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/requires: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/requires: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
examples/gaussianBlurring/CMakeFiles/Gaussian.dir/requires: examples/gaussianBlurring/CMakeFiles/Gaussian.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires

.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/requires

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/clean:
	cd /home/ziyad/Documents/Portfolio/examples/gaussianBlurring && $(CMAKE_COMMAND) -P CMakeFiles/Gaussian.dir/cmake_clean.cmake
.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/clean

examples/gaussianBlurring/CMakeFiles/Gaussian.dir/depend:
	cd /home/ziyad/Documents/Portfolio && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ziyad/Documents/Portfolio /home/ziyad/Documents/Portfolio/examples/gaussianBlurring /home/ziyad/Documents/Portfolio /home/ziyad/Documents/Portfolio/examples/gaussianBlurring /home/ziyad/Documents/Portfolio/examples/gaussianBlurring/CMakeFiles/Gaussian.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/gaussianBlurring/CMakeFiles/Gaussian.dir/depend

