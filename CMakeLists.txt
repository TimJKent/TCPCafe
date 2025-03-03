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
    ${PROJECT_SOURCE_DIR}/src/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/Widgets/SendMessageWidget.cpp
)

target_link_libraries(${PROJECT_NAME} glfw opengl32.lib wsock32 ws2_32)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    src
)