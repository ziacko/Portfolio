# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/ziyad/Documents/Projects/Portfolio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ziyad/Documents/Projects/Portfolio

# Include any dependencies generated for this target.
include examples/sharpen/CMakeFiles/Sharpen.dir/depend.make

# Include the progress variables for this target.
include examples/sharpen/CMakeFiles/Sharpen.dir/progress.make

# Include the compile flags for this target's objects.
include examples/sharpen/CMakeFiles/Sharpen.dir/flags.make

examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o: examples/sharpen/source/Sharpen.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/examples/sharpen/source/Sharpen.cpp

examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sharpen.dir/source/Sharpen.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/examples/sharpen/source/Sharpen.cpp > CMakeFiles/Sharpen.dir/source/Sharpen.cpp.i

examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sharpen.dir/source/Sharpen.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/examples/sharpen/source/Sharpen.cpp -o CMakeFiles/Sharpen.dir/source/Sharpen.cpp.s

examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o: dependencies/imgui/imgui.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui.cpp

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui.cpp > CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui.cpp -o CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: dependencies/imgui/imgui_draw.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_draw.cpp

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_draw.cpp > CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_draw.cpp -o CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: dependencies/imgui/imgui_demo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_demo.cpp

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_demo.cpp > CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_demo.cpp -o CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o: dependencies/SOIL/src/SOIL.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.c

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.c > CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.c -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o: dependencies/SOIL/src/image_DXT.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.c

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.c > CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.c -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o: dependencies/SOIL/src/image_helper.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.c

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.c > CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.c -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o: examples/sharpen/CMakeFiles/Sharpen.dir/flags.make
examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o: dependencies/SOIL/src/stb_image_aug.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.c

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.c > CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.i

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.c -o CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.s

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires:
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires
	$(MAKE) -f examples/sharpen/CMakeFiles/Sharpen.dir/build.make examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides.build
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides

examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides.build: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o

# Object files for target Sharpen
Sharpen_OBJECTS = \
"CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o" \
"CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o"

# External object files for target Sharpen
Sharpen_EXTERNAL_OBJECTS =

examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/build.make
examples/sharpen/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/sharpen/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/sharpen/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/sharpen/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/sharpen/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/sharpen/Scene: examples/sharpen/CMakeFiles/Sharpen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Scene"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Sharpen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/sharpen/CMakeFiles/Sharpen.dir/build: examples/sharpen/Scene
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/build

examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/source/Sharpen.cpp.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires
examples/sharpen/CMakeFiles/Sharpen.dir/requires: examples/sharpen/CMakeFiles/Sharpen.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/requires

examples/sharpen/CMakeFiles/Sharpen.dir/clean:
	cd /home/ziyad/Documents/Projects/Portfolio/examples/sharpen && $(CMAKE_COMMAND) -P CMakeFiles/Sharpen.dir/cmake_clean.cmake
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/clean

examples/sharpen/CMakeFiles/Sharpen.dir/depend:
	cd /home/ziyad/Documents/Projects/Portfolio && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ziyad/Documents/Projects/Portfolio /home/ziyad/Documents/Projects/Portfolio/examples/sharpen /home/ziyad/Documents/Projects/Portfolio /home/ziyad/Documents/Projects/Portfolio/examples/sharpen /home/ziyad/Documents/Projects/Portfolio/examples/sharpen/CMakeFiles/Sharpen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/sharpen/CMakeFiles/Sharpen.dir/depend

