# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace/JavaVirtualMachine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/JavaVirtualMachine/build

# Utility rule file for vt_gen.

# Include any custom commands dependencies for this target.
include src/CMakeFiles/vt_gen.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/vt_gen.dir/progress.make

vt_gen: src/CMakeFiles/vt_gen.dir/build.make
.PHONY : vt_gen

# Rule to build all files generated by this target.
src/CMakeFiles/vt_gen.dir/build: vt_gen
.PHONY : src/CMakeFiles/vt_gen.dir/build

src/CMakeFiles/vt_gen.dir/clean:
	cd /workspace/JavaVirtualMachine/build/src && $(CMAKE_COMMAND) -P CMakeFiles/vt_gen.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/vt_gen.dir/clean

src/CMakeFiles/vt_gen.dir/depend:
	cd /workspace/JavaVirtualMachine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/JavaVirtualMachine /workspace/JavaVirtualMachine/src /workspace/JavaVirtualMachine/build /workspace/JavaVirtualMachine/build/src /workspace/JavaVirtualMachine/build/src/CMakeFiles/vt_gen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/vt_gen.dir/depend

