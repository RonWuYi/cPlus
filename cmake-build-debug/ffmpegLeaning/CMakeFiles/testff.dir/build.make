# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /cygdrive/c/Users/HFD/.CLion2017.2/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/HFD/.CLion2017.2/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Work/cPlus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Work/cPlus/cmake-build-debug

# Include any dependencies generated for this target.
include ffmpegLeaning/CMakeFiles/testff.dir/depend.make

# Include the progress variables for this target.
include ffmpegLeaning/CMakeFiles/testff.dir/progress.make

# Include the compile flags for this target's objects.
include ffmpegLeaning/CMakeFiles/testff.dir/flags.make

ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o: ffmpegLeaning/CMakeFiles/testff.dir/flags.make
ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o: ../ffmpegLeaning/src/sdlExample.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Work/cPlus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o"
	cd /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testff.dir/src/sdlExample.c.o   -c /cygdrive/c/Work/cPlus/ffmpegLeaning/src/sdlExample.c

ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testff.dir/src/sdlExample.c.i"
	cd /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Work/cPlus/ffmpegLeaning/src/sdlExample.c > CMakeFiles/testff.dir/src/sdlExample.c.i

ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testff.dir/src/sdlExample.c.s"
	cd /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Work/cPlus/ffmpegLeaning/src/sdlExample.c -o CMakeFiles/testff.dir/src/sdlExample.c.s

ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.requires:

.PHONY : ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.requires

ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.provides: ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.requires
	$(MAKE) -f ffmpegLeaning/CMakeFiles/testff.dir/build.make ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.provides.build
.PHONY : ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.provides

ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.provides.build: ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o


# Object files for target testff
testff_OBJECTS = \
"CMakeFiles/testff.dir/src/sdlExample.c.o"

# External object files for target testff
testff_EXTERNAL_OBJECTS =

bin/testff.exe: ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o
bin/testff.exe: ffmpegLeaning/CMakeFiles/testff.dir/build.make
bin/testff.exe: ffmpegLeaning/CMakeFiles/testff.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Work/cPlus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../bin/testff.exe"
	cd /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testff.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ffmpegLeaning/CMakeFiles/testff.dir/build: bin/testff.exe

.PHONY : ffmpegLeaning/CMakeFiles/testff.dir/build

ffmpegLeaning/CMakeFiles/testff.dir/requires: ffmpegLeaning/CMakeFiles/testff.dir/src/sdlExample.c.o.requires

.PHONY : ffmpegLeaning/CMakeFiles/testff.dir/requires

ffmpegLeaning/CMakeFiles/testff.dir/clean:
	cd /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning && $(CMAKE_COMMAND) -P CMakeFiles/testff.dir/cmake_clean.cmake
.PHONY : ffmpegLeaning/CMakeFiles/testff.dir/clean

ffmpegLeaning/CMakeFiles/testff.dir/depend:
	cd /cygdrive/c/Work/cPlus/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Work/cPlus /cygdrive/c/Work/cPlus/ffmpegLeaning /cygdrive/c/Work/cPlus/cmake-build-debug /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning /cygdrive/c/Work/cPlus/cmake-build-debug/ffmpegLeaning/CMakeFiles/testff.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ffmpegLeaning/CMakeFiles/testff.dir/depend

