# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Laurent\Windows\ODFAEG

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Laurent\Windows\ODFAEG\build

# Include any dependencies generated for this target.
include src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/depend.make

# Include the progress variables for this target.
include src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/progress.make

# Include the compile flags for this target's objects.
include src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/flags.make

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.obj: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/flags.make
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.obj: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/includes_CXX.rsp
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.obj: ../src/odfaeg/Audio/player.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-audio.dir\player.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\player.cpp

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-audio.dir/player.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\player.cpp > CMakeFiles\odfaeg-audio.dir\player.cpp.i

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-audio.dir/player.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\player.cpp -o CMakeFiles\odfaeg-audio.dir\player.cpp.s

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.obj: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/flags.make
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.obj: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/includes_CXX.rsp
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.obj: ../src/odfaeg/Audio/stream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-audio.dir\stream.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\stream.cpp

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-audio.dir/stream.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\stream.cpp > CMakeFiles\odfaeg-audio.dir\stream.cpp.i

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-audio.dir/stream.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\stream.cpp -o CMakeFiles\odfaeg-audio.dir\stream.cpp.s

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.obj: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/flags.make
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.obj: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/includes_CXX.rsp
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.obj: ../src/odfaeg/Audio/SoundFile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-audio.dir\SoundFile.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\SoundFile.cpp

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\SoundFile.cpp > CMakeFiles\odfaeg-audio.dir\SoundFile.cpp.i

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && C:\PROGRA~2\CODEBL~1\MinGW\bin\MINGW3~2.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio\SoundFile.cpp -o CMakeFiles\odfaeg-audio.dir\SoundFile.cpp.s

# Object files for target odfaeg-audio
odfaeg__audio_OBJECTS = \
"CMakeFiles/odfaeg-audio.dir/player.cpp.obj" \
"CMakeFiles/odfaeg-audio.dir/stream.cpp.obj" \
"CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.obj"

# External object files for target odfaeg-audio
odfaeg__audio_EXTERNAL_OBJECTS =

lib/libodfaeg-audio-s-d.a: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/player.cpp.obj
lib/libodfaeg-audio-s-d.a: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/stream.cpp.obj
lib/libodfaeg-audio-s-d.a: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/SoundFile.cpp.obj
lib/libodfaeg-audio-s-d.a: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/build.make
lib/libodfaeg-audio-s-d.a: src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Laurent\Windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ..\..\..\lib\libodfaeg-audio-s-d.a"
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && $(CMAKE_COMMAND) -P CMakeFiles\odfaeg-audio.dir\cmake_clean_target.cmake
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\odfaeg-audio.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/build: lib/libodfaeg-audio-s-d.a

.PHONY : src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/build

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/clean:
	cd /d C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio && $(CMAKE_COMMAND) -P CMakeFiles\odfaeg-audio.dir\cmake_clean.cmake
.PHONY : src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/clean

src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Laurent\Windows\ODFAEG C:\Users\Laurent\Windows\ODFAEG\src\odfaeg\Audio C:\Users\Laurent\Windows\ODFAEG\build C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio C:\Users\Laurent\Windows\ODFAEG\build\src\odfaeg\Audio\CMakeFiles\odfaeg-audio.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/odfaeg/Audio/CMakeFiles/odfaeg-audio.dir/depend

