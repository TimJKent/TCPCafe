# TCPCafe
ImGUI based tool for testing TCP connections

## Demo

Node Editor View
![NodeEditorDemo.png](promo/NodeEditorDemo.png "NodeEditorDemo.png")

TCP Server View
![TCPServerDemo.png](promo/TCPServerDemo.png "TCPServerDemo.png")



## Building

### Generators
- Tested Compilers: MinGW
- MSVC: Known bug that prevents BeginCombo's from working correctly

### CMake
1. Create Directory /build
1. cd build
1. cmake -G "Generator Name" ..
1. cmake --build .
1. ./TCPCafe.exe
