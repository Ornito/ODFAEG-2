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
CMAKE_SOURCE_DIR = C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build

# Include any dependencies generated for this target.
include src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/depend.make

# Include the progress variables for this target.
include src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/progress.make

# Include the compile flags for this target's objects.
include src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.obj: ../src/odfaeg/Network/aes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\aes.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\aes.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/aes.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\aes.cpp > CMakeFiles\odfaeg-network.dir\aes.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/aes.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\aes.cpp -o CMakeFiles\odfaeg-network.dir\aes.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.obj: ../src/odfaeg/Network/rsa.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\rsa.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\rsa.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/rsa.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\rsa.cpp > CMakeFiles\odfaeg-network.dir\rsa.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/rsa.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\rsa.cpp -o CMakeFiles\odfaeg-network.dir\rsa.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.obj: ../src/odfaeg/Network/encryptedPacket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\encryptedPacket.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\encryptedPacket.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\encryptedPacket.cpp > CMakeFiles\odfaeg-network.dir\encryptedPacket.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\encryptedPacket.cpp -o CMakeFiles\odfaeg-network.dir\encryptedPacket.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.obj: ../src/odfaeg/Network/symEncPacket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\symEncPacket.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\symEncPacket.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\symEncPacket.cpp > CMakeFiles\odfaeg-network.dir\symEncPacket.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\symEncPacket.cpp -o CMakeFiles\odfaeg-network.dir\symEncPacket.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.obj: ../src/odfaeg/Network/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\client.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\client.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/client.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\client.cpp > CMakeFiles\odfaeg-network.dir\client.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/client.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\client.cpp -o CMakeFiles\odfaeg-network.dir\client.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.obj: ../src/odfaeg/Network/srkserveur.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\srkserveur.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\srkserveur.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/srkserveur.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\srkserveur.cpp > CMakeFiles\odfaeg-network.dir\srkserveur.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/srkserveur.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\srkserveur.cpp -o CMakeFiles\odfaeg-network.dir\srkserveur.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.obj: ../src/odfaeg/Network/network.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\network.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\network.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/network.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\network.cpp > CMakeFiles\odfaeg-network.dir\network.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/network.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\network.cpp -o CMakeFiles\odfaeg-network.dir\network.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.obj: ../src/odfaeg/Network/user.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\user.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\user.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/user.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\user.cpp > CMakeFiles\odfaeg-network.dir\user.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/user.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\user.cpp -o CMakeFiles\odfaeg-network.dir\user.cpp.s

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/flags.make
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.obj: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/includes_CXX.rsp
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.obj: ../src/odfaeg/Network/cliEncryptedPacket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.obj"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\odfaeg-network.dir\cliEncryptedPacket.cpp.obj -c C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\cliEncryptedPacket.cpp

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.i"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\cliEncryptedPacket.cpp > CMakeFiles\odfaeg-network.dir\cliEncryptedPacket.cpp.i

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.s"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network\cliEncryptedPacket.cpp -o CMakeFiles\odfaeg-network.dir\cliEncryptedPacket.cpp.s

# Object files for target odfaeg-network
odfaeg__network_OBJECTS = \
"CMakeFiles/odfaeg-network.dir/aes.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/rsa.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/client.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/srkserveur.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/network.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/user.cpp.obj" \
"CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.obj"

# External object files for target odfaeg-network
odfaeg__network_EXTERNAL_OBJECTS =

lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/aes.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/rsa.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/encryptedPacket.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/symEncPacket.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/client.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/srkserveur.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/network.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/user.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/cliEncryptedPacket.cpp.obj
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/build.make
lib/libodfaeg-network-s-d.a: src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library ..\..\..\lib\libodfaeg-network-s-d.a"
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && $(CMAKE_COMMAND) -P CMakeFiles\odfaeg-network.dir\cmake_clean_target.cmake
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\odfaeg-network.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/build: lib/libodfaeg-network-s-d.a

.PHONY : src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/build

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/clean:
	cd /d C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network && $(CMAKE_COMMAND) -P CMakeFiles\odfaeg-network.dir\cmake_clean.cmake
.PHONY : src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/clean

src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\src\odfaeg\Network C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network C:\Users\Laurent\Windows\ODFAEG-windows\ODFAEG\build\src\odfaeg\Network\CMakeFiles\odfaeg-network.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/odfaeg/Network/CMakeFiles/odfaeg-network.dir/depend

