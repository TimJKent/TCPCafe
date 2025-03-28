

cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
        nlohmann-json
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip
)

FetchContent_GetProperties(nlohmann-json)
if(NOT nlohmann-json_POPULATED)
    FetchContent_Populate(nlohmann-json)
endif()

set(NLOHMANN-JSON_IncludeDirectories
    ${nlohmann-json_SOURCE_DIR}/include
)