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

# Include any dependencies generated for this target.
include test/CMakeFiles/JavaVirtualMachineTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/JavaVirtualMachineTest.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/JavaVirtualMachineTest.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/JavaVirtualMachineTest.dir/flags.make

test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/flags.make
test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o: /workspace/JavaVirtualMachine/test/src/classFileTests.cpp
test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/JavaVirtualMachine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o -MF CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o.d -o CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o -c /workspace/JavaVirtualMachine/test/src/classFileTests.cpp

test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.i"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/JavaVirtualMachine/test/src/classFileTests.cpp > CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.i

test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.s"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/JavaVirtualMachine/test/src/classFileTests.cpp -o CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.s

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/flags.make
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o: /workspace/JavaVirtualMachine/src/classFile/class_file.cpp
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/JavaVirtualMachine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o -MF CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o.d -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o -c /workspace/JavaVirtualMachine/src/classFile/class_file.cpp

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.i"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/JavaVirtualMachine/src/classFile/class_file.cpp > CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.i

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.s"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/JavaVirtualMachine/src/classFile/class_file.cpp -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.s

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/flags.make
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o: /workspace/JavaVirtualMachine/src/runtime/byte_code_engine.cpp
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/JavaVirtualMachine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o -MF CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o.d -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o -c /workspace/JavaVirtualMachine/src/runtime/byte_code_engine.cpp

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.i"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/JavaVirtualMachine/src/runtime/byte_code_engine.cpp > CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.i

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.s"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/JavaVirtualMachine/src/runtime/byte_code_engine.cpp -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.s

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/flags.make
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o: /workspace/JavaVirtualMachine/src/runtime/klass.cpp
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/JavaVirtualMachine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o -MF CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o.d -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o -c /workspace/JavaVirtualMachine/src/runtime/klass.cpp

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.i"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/JavaVirtualMachine/src/runtime/klass.cpp > CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.i

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.s"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/JavaVirtualMachine/src/runtime/klass.cpp -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.s

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/flags.make
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o: /workspace/JavaVirtualMachine/src/runtime/oop.cpp
test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o: test/CMakeFiles/JavaVirtualMachineTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/JavaVirtualMachine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o -MF CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o.d -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o -c /workspace/JavaVirtualMachine/src/runtime/oop.cpp

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.i"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/JavaVirtualMachine/src/runtime/oop.cpp > CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.i

test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.s"
	cd /workspace/JavaVirtualMachine/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/JavaVirtualMachine/src/runtime/oop.cpp -o CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.s

# Object files for target JavaVirtualMachineTest
JavaVirtualMachineTest_OBJECTS = \
"CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o" \
"CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o" \
"CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o" \
"CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o" \
"CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o"

# External object files for target JavaVirtualMachineTest
JavaVirtualMachineTest_EXTERNAL_OBJECTS =

/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/src/classFileTests.cpp.o
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/classFile/class_file.cpp.o
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/byte_code_engine.cpp.o
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/klass.cpp.o
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/__/src/runtime/oop.cpp.o
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/build.make
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: /usr/lib/x86_64-linux-gnu/libgtest_main.a
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: /usr/lib/x86_64-linux-gnu/libgtest.a
/workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest: test/CMakeFiles/JavaVirtualMachineTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspace/JavaVirtualMachine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable /workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest"
	cd /workspace/JavaVirtualMachine/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/JavaVirtualMachineTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/JavaVirtualMachineTest.dir/build: /workspace/JavaVirtualMachine/test/bin/JavaVirtualMachineTest
.PHONY : test/CMakeFiles/JavaVirtualMachineTest.dir/build

test/CMakeFiles/JavaVirtualMachineTest.dir/clean:
	cd /workspace/JavaVirtualMachine/build/test && $(CMAKE_COMMAND) -P CMakeFiles/JavaVirtualMachineTest.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/JavaVirtualMachineTest.dir/clean

test/CMakeFiles/JavaVirtualMachineTest.dir/depend:
	cd /workspace/JavaVirtualMachine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/JavaVirtualMachine /workspace/JavaVirtualMachine/test /workspace/JavaVirtualMachine/build /workspace/JavaVirtualMachine/build/test /workspace/JavaVirtualMachine/build/test/CMakeFiles/JavaVirtualMachineTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/JavaVirtualMachineTest.dir/depend

