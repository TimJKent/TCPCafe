cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
        asio
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL  https://sourceforge.net/projects/asio/files/latest/download)

FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()

set(ASIO_IncludeDirectories
    ${asio_SOURCE_DIR}/include
)