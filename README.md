# TCPCafe
ImGUI based tool for testing TCP connections

## Building

### CMake Generators
CMake will select a build system automatically base on your OS.
If you wish to select a different build system:
1. mkdir /build (delete if already exists)
1. cd /build
1. View available Build System: cmake -G
1. Generate Build Files: cmake -G "Generator Name" ..

### Building
1. cd build
1. cmake --build .
