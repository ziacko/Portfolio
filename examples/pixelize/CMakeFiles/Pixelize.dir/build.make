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
include examples/pixelize/CMakeFiles/Pixelize.dir/depend.make

# Include the progress variables for this target.
include examples/pixelize/CMakeFiles/Pixelize.dir/progress.make

# Include the compile flags for this target's objects.
include examples/pixelize/CMakeFiles/Pixelize.dir/flags.make

examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o: examples/pixelize/CMakeFiles/Pixelize.dir/flags.make
examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o: examples/pixelize/source/textured.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Pixelize.dir/source/textured.cpp.o -c /home/ziyad/Documents/Portfolio/examples/pixelize/source/textured.cpp

examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Pixelize.dir/source/textured.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/examples/pixelize/source/textured.cpp > CMakeFiles/Pixelize.dir/source/textured.cpp.i

examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Pixelize.dir/source/textured.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/examples/pixelize/source/textured.cpp -o CMakeFiles/Pixelize.dir/source/textured.cpp.s

examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.requires:

.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.requires

examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.provides: examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.requires
	$(MAKE) -f examples/pixelize/CMakeFiles/Pixelize.dir/build.make examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.provides.build
.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.provides

examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.provides.build: examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o


examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o: examples/pixelize/CMakeFiles/Pixelize.dir/flags.make
examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o: dependencies/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o -c /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui.cpp

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui.cpp > CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.i

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui.cpp -o CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.s

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.requires:

.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.requires

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.provides: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
	$(MAKE) -f examples/pixelize/CMakeFiles/Pixelize.dir/build.make examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build
.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.provides

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o


examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: examples/pixelize/CMakeFiles/Pixelize.dir/flags.make
examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: dependencies/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o -c /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_draw.cpp

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_draw.cpp > CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.i

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_draw.cpp -o CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.s

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires:

.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
	$(MAKE) -f examples/pixelize/CMakeFiles/Pixelize.dir/build.make examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build
.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o


examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: examples/pixelize/CMakeFiles/Pixelize.dir/flags.make
examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: dependencies/imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o -c /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_demo.cpp

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.i"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_demo.cpp > CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.i

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.s"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziyad/Documents/Portfolio/dependencies/imgui/imgui_demo.cpp -o CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.s

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires:

.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires
	$(MAKE) -f examples/pixelize/CMakeFiles/Pixelize.dir/build.make examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build
.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides

examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o


# Object files for target Pixelize
Pixelize_OBJECTS = \
"CMakeFiles/Pixelize.dir/source/textured.cpp.o" \
"CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o" \
"CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o" \
"CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o"

# External object files for target Pixelize
Pixelize_EXTERNAL_OBJECTS =

examples/pixelize/Scene: examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o
examples/pixelize/Scene: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o
examples/pixelize/Scene: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o
examples/pixelize/Scene: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o
examples/pixelize/Scene: examples/pixelize/CMakeFiles/Pixelize.dir/build.make
examples/pixelize/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/pixelize/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/pixelize/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/pixelize/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/pixelize/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/pixelize/Scene: examples/pixelize/CMakeFiles/Pixelize.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ziyad/Documents/Portfolio/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Scene"
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Pixelize.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/pixelize/CMakeFiles/Pixelize.dir/build: examples/pixelize/Scene

.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/build

examples/pixelize/CMakeFiles/Pixelize.dir/requires: examples/pixelize/CMakeFiles/Pixelize.dir/source/textured.cpp.o.requires
examples/pixelize/CMakeFiles/Pixelize.dir/requires: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
examples/pixelize/CMakeFiles/Pixelize.dir/requires: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
examples/pixelize/CMakeFiles/Pixelize.dir/requires: examples/pixelize/CMakeFiles/Pixelize.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires

.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/requires

examples/pixelize/CMakeFiles/Pixelize.dir/clean:
	cd /home/ziyad/Documents/Portfolio/examples/pixelize && $(CMAKE_COMMAND) -P CMakeFiles/Pixelize.dir/cmake_clean.cmake
.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/clean

examples/pixelize/CMakeFiles/Pixelize.dir/depend:
	cd /home/ziyad/Documents/Portfolio && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ziyad/Documents/Portfolio /home/ziyad/Documents/Portfolio/examples/pixelize /home/ziyad/Documents/Portfolio /home/ziyad/Documents/Portfolio/examples/pixelize /home/ziyad/Documents/Portfolio/examples/pixelize/CMakeFiles/Pixelize.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/pixelize/CMakeFiles/Pixelize.dir/depend

