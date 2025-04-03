cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
        stb
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY      https://github.com/nothings/stb
        )

FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
endif()

set(stb_IncludeDirectories
    ${stb_SOURCE_DIR}
)