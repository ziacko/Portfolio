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
include examples/gameOfLife/CMakeFiles/gameOfLife.dir/depend.make

# Include the progress variables for this target.
include examples/gameOfLife/CMakeFiles/gameOfLife.dir/progress.make

# Include the compile flags for this target's objects.
include examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make

examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o: examples/gameOfLife/source/scene.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gameOfLife.dir/source/scene.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife/source/scene.cpp

examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gameOfLife.dir/source/scene.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife/source/scene.cpp > CMakeFiles/gameOfLife.dir/source/scene.cpp.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gameOfLife.dir/source/scene.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife/source/scene.cpp -o CMakeFiles/gameOfLife.dir/source/scene.cpp.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o: dependencies/imgui/imgui.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui.cpp

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui.cpp > CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui.cpp -o CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o: dependencies/imgui/imgui_draw.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_draw.cpp

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_draw.cpp > CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_draw.cpp -o CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o: dependencies/imgui/imgui_demo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o -c /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_demo.cpp

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_demo.cpp > CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/imgui/imgui_demo.cpp -o CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o: dependencies/SOIL/src/SOIL.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.c

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.c > CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/SOIL.c -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o: dependencies/SOIL/src/image_DXT.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.c

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.c > CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_DXT.c -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o: dependencies/SOIL/src/image_helper.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.c

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.c > CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/image_helper.c -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o: examples/gameOfLife/CMakeFiles/gameOfLife.dir/flags.make
examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o: dependencies/SOIL/src/stb_image_aug.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ziyad/Documents/Projects/Portfolio/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o   -c /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.c

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.i"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.c > CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.i

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.s"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/ziyad/Documents/Projects/Portfolio/dependencies/SOIL/src/stb_image_aug.c -o CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.s

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires:
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires
	$(MAKE) -f examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides.build
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides

examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.provides.build: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o

# Object files for target gameOfLife
gameOfLife_OBJECTS = \
"CMakeFiles/gameOfLife.dir/source/scene.cpp.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o" \
"CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o"

# External object files for target gameOfLife
gameOfLife_EXTERNAL_OBJECTS =

examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/build.make
examples/gameOfLife/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/gameOfLife/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/gameOfLife/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/gameOfLife/Scene: /usr/lib/x86_64-linux-gnu/libGL.so
examples/gameOfLife/Scene: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/gameOfLife/Scene: examples/gameOfLife/CMakeFiles/gameOfLife.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Scene"
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gameOfLife.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/gameOfLife/CMakeFiles/gameOfLife.dir/build: examples/gameOfLife/Scene
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/build

examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/source/scene.cpp.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui.cpp.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_draw.cpp.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/imgui/imgui_demo.cpp.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/SOIL.c.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_DXT.c.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/image_helper.c.o.requires
examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires: examples/gameOfLife/CMakeFiles/gameOfLife.dir/__/__/dependencies/SOIL/src/stb_image_aug.c.o.requires
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/requires

examples/gameOfLife/CMakeFiles/gameOfLife.dir/clean:
	cd /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife && $(CMAKE_COMMAND) -P CMakeFiles/gameOfLife.dir/cmake_clean.cmake
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/clean

examples/gameOfLife/CMakeFiles/gameOfLife.dir/depend:
	cd /home/ziyad/Documents/Projects/Portfolio && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ziyad/Documents/Projects/Portfolio /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife /home/ziyad/Documents/Projects/Portfolio /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife /home/ziyad/Documents/Projects/Portfolio/examples/gameOfLife/CMakeFiles/gameOfLife.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/gameOfLife/CMakeFiles/gameOfLife.dir/depend

