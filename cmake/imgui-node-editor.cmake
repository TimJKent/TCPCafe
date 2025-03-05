cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
        imgui_node_editor
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
        GIT_TAG "develop"
)

FetchContent_GetProperties(imgui_node_editor)
if(NOT imgui_node_editor_POPULATED)
    FetchContent_Populate(imgui_node_editor)
endif()

set(IMGUI_NODE_EDITOR_IncludeDirectories
    ${imgui_node_editor_SOURCE_DIR}
)

set(IMGUI_NODE_EDITOR_Sources
    ${imgui_node_editor_SOURCE_DIR}/crude_json.cpp
    ${imgui_node_editor_SOURCE_DIR}/imgui_bezier_math.inl
    ${imgui_node_editor_SOURCE_DIR}/imgui_canvas.cpp
    ${imgui_node_editor_SOURCE_DIR}/imgui_extra_math.inl
    ${imgui_node_editor_SOURCE_DIR}/imgui_node_editor.cpp
    ${imgui_node_editor_SOURCE_DIR}/imgui_node_editor_api.cpp
    ${imgui_node_editor_SOURCE_DIR}/imgui_node_editor_internal.inl
)