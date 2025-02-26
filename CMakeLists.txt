cmake_minimum_required(VERSION 4.0)
project(TCPCafe)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(GLFW_STANDALONE true)

add_definitions(-DASIO_STANDALONE )

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_sources(${PROJECT_NAME}
PRIVATE
    ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c

    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/TCP/Client/TCPClient.cpp
    ${PROJECT_SOURCE_DIR}/src/TCP/Server/TCPServer.cpp
    ${PROJECT_SOURCE_DIR}/src/TCP/Server/TCPConnection.cpp
    ${PROJECT_SOURCE_DIR}/src/Window/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/Application.cpp

    
)

target_link_libraries(${PROJECT_NAME} glfw opengl32.lib wsock32 ws2_32)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    src
    vendor/glfw/include
    vendor/imgui
    vendor/imgui/backends
    vendor/glad/include
    vendor/asio/include
)

add_subdirectory(vendor/GLFW)