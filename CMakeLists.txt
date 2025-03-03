cmake_minimum_required(VERSION 4.0)
include(FetchContent)

project(TCPCafe)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL
find_package(OpenGL REQUIRED)
cmake_policy(SET CMP0169 OLD)

# GLFW
find_package(glfw3 3.4 QUIET)
if (NOT glfw3_FOUND)
    FetchContent_Declare(
            glfw3
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    )
    FetchContent_GetProperties(glfw3)
    if (NOT glfw3_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(glfw3)
        add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
    endif()
endif()


# GLAD
FetchContent_Declare(
    glad
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/Dav1dde/glad/archive/refs/tags/v2.0.8.zip
)


FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6)
endif()
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# ImGui
find_package(imgui 1.91.8 QUIET)
if (NOT imgui_FOUND)
    FetchContent_Declare(
            imgui
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.8.zip
    )
endif()

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
    vendor/imgui
    vendor/imgui/backends
    vendor/glad/include
    vendor/asio/include
)