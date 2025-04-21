cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
        imgui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY      https://github.com/pthom/imgui.git
        GIT_TAG "imgui_bundle"
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

set(IMGUI_IncludeDirectories
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

set(IMGUI_Sources
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_stacklayout.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp

)