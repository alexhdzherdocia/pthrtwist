# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rhesus/pthrtwist

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rhesus/pthrtwist/build

# Include any dependencies generated for this target.
include CMakeFiles/pthrtwist.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pthrtwist.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pthrtwist.dir/flags.make

CMakeFiles/pthrtwist.dir/main.c.o: CMakeFiles/pthrtwist.dir/flags.make
CMakeFiles/pthrtwist.dir/main.c.o: ../main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/rhesus/pthrtwist/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/pthrtwist.dir/main.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/pthrtwist.dir/main.c.o   -c /home/rhesus/pthrtwist/main.c

CMakeFiles/pthrtwist.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pthrtwist.dir/main.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/rhesus/pthrtwist/main.c > CMakeFiles/pthrtwist.dir/main.c.i

CMakeFiles/pthrtwist.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pthrtwist.dir/main.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/rhesus/pthrtwist/main.c -o CMakeFiles/pthrtwist.dir/main.c.s

CMakeFiles/pthrtwist.dir/main.c.o.requires:
.PHONY : CMakeFiles/pthrtwist.dir/main.c.o.requires

CMakeFiles/pthrtwist.dir/main.c.o.provides: CMakeFiles/pthrtwist.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/pthrtwist.dir/build.make CMakeFiles/pthrtwist.dir/main.c.o.provides.build
.PHONY : CMakeFiles/pthrtwist.dir/main.c.o.provides

CMakeFiles/pthrtwist.dir/main.c.o.provides.build: CMakeFiles/pthrtwist.dir/main.c.o

# Object files for target pthrtwist
pthrtwist_OBJECTS = \
"CMakeFiles/pthrtwist.dir/main.c.o"

# External object files for target pthrtwist
pthrtwist_EXTERNAL_OBJECTS =

pthrtwist: CMakeFiles/pthrtwist.dir/main.c.o
pthrtwist: CMakeFiles/pthrtwist.dir/build.make
pthrtwist: CMakeFiles/pthrtwist.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable pthrtwist"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pthrtwist.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pthrtwist.dir/build: pthrtwist
.PHONY : CMakeFiles/pthrtwist.dir/build

CMakeFiles/pthrtwist.dir/requires: CMakeFiles/pthrtwist.dir/main.c.o.requires
.PHONY : CMakeFiles/pthrtwist.dir/requires

CMakeFiles/pthrtwist.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pthrtwist.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pthrtwist.dir/clean

CMakeFiles/pthrtwist.dir/depend:
	cd /home/rhesus/pthrtwist/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rhesus/pthrtwist /home/rhesus/pthrtwist /home/rhesus/pthrtwist/build /home/rhesus/pthrtwist/build /home/rhesus/pthrtwist/build/CMakeFiles/pthrtwist.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pthrtwist.dir/depend
