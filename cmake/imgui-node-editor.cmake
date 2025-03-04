cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
        imgui-node-editor
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY  https://github.com/thedmd/imgui-node-editor.git
        GIT_TAG "develop"
)

FetchContent_GetProperties(imgui-node-editor)
if(NOT imgui-node-editor_POPULATED)
    FetchContent_Populate(imgui-node-editor)
endif()

set(IMGUI_NODE_EDITOR_IncludeDirectories
    ${imgui-node-editor_SOURCE_DIR}/crude_json.h
    ${imgui-node-editor_SOURCE_DIR}/imgui_bezier_math.h
    ${imgui-node-editor_SOURCE_DIR}/imgui_canvas.h
    ${imgui-node-editor_SOURCE_DIR}/imgui_extra_math.h
    ${imgui-node-editor_SOURCE_DIR}/imgui_node_editor.h
    ${imgui-node-editor_SOURCE_DIR}/imgui_node_editor_internal.h
)

set(IMGUI_NODE_EDITOR_Sources
    ${imgui-node-editor_SOURCE_DIR}/crude_json.cpp
    ${imgui-node-editor_SOURCE_DIR}/imgui_bezier_math.inl
    ${imgui-node-editor_SOURCE_DIR}/imgui_canvas.cpp
    ${imgui-node-editor_SOURCE_DIR}/imgui_extra_math.inl
    ${imgui-node-editor_SOURCE_DIR}/imgui_node_editor.cpp
    ${imgui-node-editor_SOURCE_DIR}/imgui_node_editor_api.cpp
    ${imgui-node-editor_SOURCE_DIR}/imgui_node_editor_internal.inl
)