# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_COMMAND = /snap/cmake/1457/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1457/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build

# Include any dependencies generated for this target.
include CMakeFiles/hw_cpusched.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hw_cpusched.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hw_cpusched.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw_cpusched.dir/flags.make

CMakeFiles/hw_cpusched.dir/codegen:
.PHONY : CMakeFiles/hw_cpusched.dir/codegen

CMakeFiles/hw_cpusched.dir/src/functions.c.o: CMakeFiles/hw_cpusched.dir/flags.make
CMakeFiles/hw_cpusched.dir/src/functions.c.o: /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/functions.c
CMakeFiles/hw_cpusched.dir/src/functions.c.o: CMakeFiles/hw_cpusched.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw_cpusched.dir/src/functions.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/hw_cpusched.dir/src/functions.c.o -MF CMakeFiles/hw_cpusched.dir/src/functions.c.o.d -o CMakeFiles/hw_cpusched.dir/src/functions.c.o -c /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/functions.c

CMakeFiles/hw_cpusched.dir/src/functions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/hw_cpusched.dir/src/functions.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/functions.c > CMakeFiles/hw_cpusched.dir/src/functions.c.i

CMakeFiles/hw_cpusched.dir/src/functions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/hw_cpusched.dir/src/functions.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/functions.c -o CMakeFiles/hw_cpusched.dir/src/functions.c.s

CMakeFiles/hw_cpusched.dir/src/main.c.o: CMakeFiles/hw_cpusched.dir/flags.make
CMakeFiles/hw_cpusched.dir/src/main.c.o: /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/main.c
CMakeFiles/hw_cpusched.dir/src/main.c.o: CMakeFiles/hw_cpusched.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/hw_cpusched.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/hw_cpusched.dir/src/main.c.o -MF CMakeFiles/hw_cpusched.dir/src/main.c.o.d -o CMakeFiles/hw_cpusched.dir/src/main.c.o -c /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/main.c

CMakeFiles/hw_cpusched.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/hw_cpusched.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/main.c > CMakeFiles/hw_cpusched.dir/src/main.c.i

CMakeFiles/hw_cpusched.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/hw_cpusched.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/src/main.c -o CMakeFiles/hw_cpusched.dir/src/main.c.s

# Object files for target hw_cpusched
hw_cpusched_OBJECTS = \
"CMakeFiles/hw_cpusched.dir/src/functions.c.o" \
"CMakeFiles/hw_cpusched.dir/src/main.c.o"

# External object files for target hw_cpusched
hw_cpusched_EXTERNAL_OBJECTS =

hw_cpusched: CMakeFiles/hw_cpusched.dir/src/functions.c.o
hw_cpusched: CMakeFiles/hw_cpusched.dir/src/main.c.o
hw_cpusched: CMakeFiles/hw_cpusched.dir/build.make
hw_cpusched: CMakeFiles/hw_cpusched.dir/compiler_depend.ts
hw_cpusched: CMakeFiles/hw_cpusched.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable hw_cpusched"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw_cpusched.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw_cpusched.dir/build: hw_cpusched
.PHONY : CMakeFiles/hw_cpusched.dir/build

CMakeFiles/hw_cpusched.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw_cpusched.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw_cpusched.dir/clean

CMakeFiles/hw_cpusched.dir/depend:
	cd /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build /home/jj/Documents/Classwork/OS_Classwork/HW_CPUSCHED/build/CMakeFiles/hw_cpusched.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/hw_cpusched.dir/depend

