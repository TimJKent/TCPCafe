cmake_minimum_required(VERSION 3.11)
include(FetchContent)

project(TCPCafe)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DASIO_STANDALONE )

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)

#BEGIN_DEPS

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
include(cmake/glfw.cmake)

# Glad
include(cmake/glad.cmake)

# Dear ImGui
include(cmake/imgui.cmake)
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_IncludeDirectories})
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_Sources})

# Asio
include(cmake/asio.cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ${ASIO_IncludeDirectories})

# imgui-node-editor
include(cmake/imgui-node-editor.cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_NODE_EDITOR_IncludeDirectories})
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_NODE_EDITOR_Sources})

# nlohmann-json
include(cmake/nlohmann-json.cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ${NLOHMANN-JSON_IncludeDirectories})

# stb
include(cmake/stb.cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ${stb_IncludeDirectories})

include(cmake/nativefiledialog-extended.cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ${NATIVEFILEDIALOG-EXTENDED_IncludeDirectories})

#END_DEPS

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_sources(${PROJECT_NAME}
PRIVATE
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/TCP/Client/TCPClient.cpp
    ${PROJECT_SOURCE_DIR}/src/TCP/Server/TCPServer.cpp
    ${PROJECT_SOURCE_DIR}/src/TCP/Server/TCPConnection.cpp
    ${PROJECT_SOURCE_DIR}/src/Window/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/Window/FileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/Widgets/SendMessageWidget.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/PrintNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/Pin.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/ButtonNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/BooleanOperatorNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/BooleanDisplayNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/ToggleNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/TimerNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/AddNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/SubtractNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/StringNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/NumberNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/ConcatNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/TCPClientNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/TCPServerNode.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/Node.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/NodeManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/Widgets.cpp
    ${PROJECT_SOURCE_DIR}/src/Nodes/Drawing.cpp
)

file(COPY
    ${CMAKE_SOURCE_DIR}/assets/icon.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} glfw opengl32.lib wsock32 ws2_32 nfd) 

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    src
)