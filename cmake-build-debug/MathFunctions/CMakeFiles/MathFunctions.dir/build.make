# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/hdc/Downloads/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/hdc/Downloads/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hdc/CLionProjects/cPlus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hdc/CLionProjects/cPlus/cmake-build-debug

# Include any dependencies generated for this target.
include MathFunctions/CMakeFiles/MathFunctions.dir/depend.make

# Include the progress variables for this target.
include MathFunctions/CMakeFiles/MathFunctions.dir/progress.make

# Include the compile flags for this target's objects.
include MathFunctions/CMakeFiles/MathFunctions.dir/flags.make

MathFunctions/CMakeFiles/MathFunctions.dir/mysqrt.cpp.o: MathFunctions/CMakeFiles/MathFunctions.dir/flags.make
MathFunctions/CMakeFiles/MathFunctions.dir/mysqrt.cpp.o: ../MathFunctions/mysqrt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hdc/CLionProjects/cPlus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object MathFunctions/CMakeFiles/MathFunctions.dir/mysqrt.cpp.o"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MathFunctions.dir/mysqrt.cpp.o -c /home/hdc/CLionProjects/cPlus/MathFunctions/mysqrt.cpp

MathFunctions/CMakeFiles/MathFunctions.dir/mysqrt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MathFunctions.dir/mysqrt.cpp.i"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hdc/CLionProjects/cPlus/MathFunctions/mysqrt.cpp > CMakeFiles/MathFunctions.dir/mysqrt.cpp.i

MathFunctions/CMakeFiles/MathFunctions.dir/mysqrt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MathFunctions.dir/mysqrt.cpp.s"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hdc/CLionProjects/cPlus/MathFunctions/mysqrt.cpp -o CMakeFiles/MathFunctions.dir/mysqrt.cpp.s

MathFunctions/CMakeFiles/MathFunctions.dir/MakeTable.cpp.o: MathFunctions/CMakeFiles/MathFunctions.dir/flags.make
MathFunctions/CMakeFiles/MathFunctions.dir/MakeTable.cpp.o: ../MathFunctions/MakeTable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hdc/CLionProjects/cPlus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object MathFunctions/CMakeFiles/MathFunctions.dir/MakeTable.cpp.o"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MathFunctions.dir/MakeTable.cpp.o -c /home/hdc/CLionProjects/cPlus/MathFunctions/MakeTable.cpp

MathFunctions/CMakeFiles/MathFunctions.dir/MakeTable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MathFunctions.dir/MakeTable.cpp.i"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hdc/CLionProjects/cPlus/MathFunctions/MakeTable.cpp > CMakeFiles/MathFunctions.dir/MakeTable.cpp.i

MathFunctions/CMakeFiles/MathFunctions.dir/MakeTable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MathFunctions.dir/MakeTable.cpp.s"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hdc/CLionProjects/cPlus/MathFunctions/MakeTable.cpp -o CMakeFiles/MathFunctions.dir/MakeTable.cpp.s

# Object files for target MathFunctions
MathFunctions_OBJECTS = \
"CMakeFiles/MathFunctions.dir/mysqrt.cpp.o" \
"CMakeFiles/MathFunctions.dir/MakeTable.cpp.o"

# External object files for target MathFunctions
MathFunctions_EXTERNAL_OBJECTS =

MathFunctions/libMathFunctions.a: MathFunctions/CMakeFiles/MathFunctions.dir/mysqrt.cpp.o
MathFunctions/libMathFunctions.a: MathFunctions/CMakeFiles/MathFunctions.dir/MakeTable.cpp.o
MathFunctions/libMathFunctions.a: MathFunctions/CMakeFiles/MathFunctions.dir/build.make
MathFunctions/libMathFunctions.a: MathFunctions/CMakeFiles/MathFunctions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hdc/CLionProjects/cPlus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libMathFunctions.a"
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && $(CMAKE_COMMAND) -P CMakeFiles/MathFunctions.dir/cmake_clean_target.cmake
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MathFunctions.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
MathFunctions/CMakeFiles/MathFunctions.dir/build: MathFunctions/libMathFunctions.a

.PHONY : MathFunctions/CMakeFiles/MathFunctions.dir/build

MathFunctions/CMakeFiles/MathFunctions.dir/clean:
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions && $(CMAKE_COMMAND) -P CMakeFiles/MathFunctions.dir/cmake_clean.cmake
.PHONY : MathFunctions/CMakeFiles/MathFunctions.dir/clean

MathFunctions/CMakeFiles/MathFunctions.dir/depend:
	cd /home/hdc/CLionProjects/cPlus/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hdc/CLionProjects/cPlus /home/hdc/CLionProjects/cPlus/MathFunctions /home/hdc/CLionProjects/cPlus/cmake-build-debug /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions /home/hdc/CLionProjects/cPlus/cmake-build-debug/MathFunctions/CMakeFiles/MathFunctions.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : MathFunctions/CMakeFiles/MathFunctions.dir/depend

