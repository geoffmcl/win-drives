# win-drives Projects - CONSOLE APPLICATION

A Windows only console app, to **explore** the drives present on the system... default is to just show the logical letter, total disk size and free, reported by the `GetDiskFreeSpaceEx` API... 

Bump verbosity, to at least `-v5`, to see more information obtained by using disk driver IOCTL, `DeviceIoControl` ...

Just an information app for Windows **ONLY**. Use utilities like `$ df -H | grep -v /dev/loop`, with many other options, in **unix** like systems.

### History:

    - 20200830 - Move to DELL03, and 64-bits
    - 20160901 - updated to use cmake
    - 20100630 - original by MSVC AppWizard. 

### Files:

    - README.md - this file
    - CMakeLists.txt - root cmake 

    - src\drives.cxx
    - src\drives.hxx
    - src\drives_io.cxx
    - src\drives_io.hxx

    - build\build-me.bat

### Build:

    - cd win-drives # into the source directory...
    - cd build # into the out-of-src build folder...
    - cmake .. [options] # configure, and generate, the default build system.
    - cmake --build . --config Release # to actually build the app.
    
 Maybe the build-me.bat will be suitable for your instalation, to build both Debug and Release configurations...
 
 
; eof - 20200830 - 20160901
