# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Peter\Documents\HavingFun\NiftyCounter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Peter\Documents\HavingFun\NiftyCounter\build

# Include any dependencies generated for this target.
include CMakeFiles/NiftyCounter.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NiftyCounter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NiftyCounter.dir/flags.make

CMakeFiles/NiftyCounter.dir/main.cpp.obj: CMakeFiles/NiftyCounter.dir/flags.make
CMakeFiles/NiftyCounter.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Peter\Documents\HavingFun\NiftyCounter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NiftyCounter.dir/main.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\NiftyCounter.dir\main.cpp.obj -c C:\Users\Peter\Documents\HavingFun\NiftyCounter\main.cpp

CMakeFiles/NiftyCounter.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NiftyCounter.dir/main.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Peter\Documents\HavingFun\NiftyCounter\main.cpp > CMakeFiles\NiftyCounter.dir\main.cpp.i

CMakeFiles/NiftyCounter.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NiftyCounter.dir/main.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Peter\Documents\HavingFun\NiftyCounter\main.cpp -o CMakeFiles\NiftyCounter.dir\main.cpp.s

CMakeFiles/NiftyCounter.dir/Initializer.cpp.obj: CMakeFiles/NiftyCounter.dir/flags.make
CMakeFiles/NiftyCounter.dir/Initializer.cpp.obj: ../Initializer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Peter\Documents\HavingFun\NiftyCounter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/NiftyCounter.dir/Initializer.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\NiftyCounter.dir\Initializer.cpp.obj -c C:\Users\Peter\Documents\HavingFun\NiftyCounter\Initializer.cpp

CMakeFiles/NiftyCounter.dir/Initializer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NiftyCounter.dir/Initializer.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Peter\Documents\HavingFun\NiftyCounter\Initializer.cpp > CMakeFiles\NiftyCounter.dir\Initializer.cpp.i

CMakeFiles/NiftyCounter.dir/Initializer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NiftyCounter.dir/Initializer.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Peter\Documents\HavingFun\NiftyCounter\Initializer.cpp -o CMakeFiles\NiftyCounter.dir\Initializer.cpp.s

CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.obj: CMakeFiles/NiftyCounter.dir/flags.make
CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.obj: ../UseSingleton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Peter\Documents\HavingFun\NiftyCounter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\NiftyCounter.dir\UseSingleton.cpp.obj -c C:\Users\Peter\Documents\HavingFun\NiftyCounter\UseSingleton.cpp

CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Peter\Documents\HavingFun\NiftyCounter\UseSingleton.cpp > CMakeFiles\NiftyCounter.dir\UseSingleton.cpp.i

CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Peter\Documents\HavingFun\NiftyCounter\UseSingleton.cpp -o CMakeFiles\NiftyCounter.dir\UseSingleton.cpp.s

# Object files for target NiftyCounter
NiftyCounter_OBJECTS = \
"CMakeFiles/NiftyCounter.dir/main.cpp.obj" \
"CMakeFiles/NiftyCounter.dir/Initializer.cpp.obj" \
"CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.obj"

# External object files for target NiftyCounter
NiftyCounter_EXTERNAL_OBJECTS =

NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/main.cpp.obj
NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/Initializer.cpp.obj
NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/UseSingleton.cpp.obj
NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/build.make
NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/linklibs.rsp
NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/objects1.rsp
NiftyCounter.exe: CMakeFiles/NiftyCounter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Peter\Documents\HavingFun\NiftyCounter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable NiftyCounter.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\NiftyCounter.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NiftyCounter.dir/build: NiftyCounter.exe

.PHONY : CMakeFiles/NiftyCounter.dir/build

CMakeFiles/NiftyCounter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\NiftyCounter.dir\cmake_clean.cmake
.PHONY : CMakeFiles/NiftyCounter.dir/clean

CMakeFiles/NiftyCounter.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Peter\Documents\HavingFun\NiftyCounter C:\Users\Peter\Documents\HavingFun\NiftyCounter C:\Users\Peter\Documents\HavingFun\NiftyCounter\build C:\Users\Peter\Documents\HavingFun\NiftyCounter\build C:\Users\Peter\Documents\HavingFun\NiftyCounter\build\CMakeFiles\NiftyCounter.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NiftyCounter.dir/depend

