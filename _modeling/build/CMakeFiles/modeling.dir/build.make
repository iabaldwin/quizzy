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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.8.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.8.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/esevalero/Desktop/OnePointNav/modeling

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/esevalero/Desktop/OnePointNav/modeling/build

# Include any dependencies generated for this target.
include CMakeFiles/modeling.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/modeling.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/modeling.dir/flags.make

CMakeFiles/modeling.dir/src/modeling.cc.o: CMakeFiles/modeling.dir/flags.make
CMakeFiles/modeling.dir/src/modeling.cc.o: ../src/modeling.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/esevalero/Desktop/OnePointNav/modeling/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/modeling.dir/src/modeling.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/modeling.dir/src/modeling.cc.o -c /Users/esevalero/Desktop/OnePointNav/modeling/src/modeling.cc

CMakeFiles/modeling.dir/src/modeling.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/modeling.dir/src/modeling.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/esevalero/Desktop/OnePointNav/modeling/src/modeling.cc > CMakeFiles/modeling.dir/src/modeling.cc.i

CMakeFiles/modeling.dir/src/modeling.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/modeling.dir/src/modeling.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/esevalero/Desktop/OnePointNav/modeling/src/modeling.cc -o CMakeFiles/modeling.dir/src/modeling.cc.s

CMakeFiles/modeling.dir/src/modeling.cc.o.requires:

.PHONY : CMakeFiles/modeling.dir/src/modeling.cc.o.requires

CMakeFiles/modeling.dir/src/modeling.cc.o.provides: CMakeFiles/modeling.dir/src/modeling.cc.o.requires
	$(MAKE) -f CMakeFiles/modeling.dir/build.make CMakeFiles/modeling.dir/src/modeling.cc.o.provides.build
.PHONY : CMakeFiles/modeling.dir/src/modeling.cc.o.provides

CMakeFiles/modeling.dir/src/modeling.cc.o.provides.build: CMakeFiles/modeling.dir/src/modeling.cc.o


# Object files for target modeling
modeling_OBJECTS = \
"CMakeFiles/modeling.dir/src/modeling.cc.o"

# External object files for target modeling
modeling_EXTERNAL_OBJECTS =

modeling: CMakeFiles/modeling.dir/src/modeling.cc.o
modeling: CMakeFiles/modeling.dir/build.make
modeling: CMakeFiles/modeling.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/esevalero/Desktop/OnePointNav/modeling/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable modeling"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/modeling.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/modeling.dir/build: modeling

.PHONY : CMakeFiles/modeling.dir/build

CMakeFiles/modeling.dir/requires: CMakeFiles/modeling.dir/src/modeling.cc.o.requires

.PHONY : CMakeFiles/modeling.dir/requires

CMakeFiles/modeling.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/modeling.dir/cmake_clean.cmake
.PHONY : CMakeFiles/modeling.dir/clean

CMakeFiles/modeling.dir/depend:
	cd /Users/esevalero/Desktop/OnePointNav/modeling/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/esevalero/Desktop/OnePointNav/modeling /Users/esevalero/Desktop/OnePointNav/modeling /Users/esevalero/Desktop/OnePointNav/modeling/build /Users/esevalero/Desktop/OnePointNav/modeling/build /Users/esevalero/Desktop/OnePointNav/modeling/build/CMakeFiles/modeling.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/modeling.dir/depend

