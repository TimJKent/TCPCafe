cmake_minimum_required(VERSION 3.11)
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

# nativefiledialog-extended
find_package(nativefiledialog-extended 1.2.1 QUIET)
if (NOT gnativefiledialog-extended_FOUND)
    FetchContent_Declare(
        nativefiledialog-extended
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/btzy/nativefiledialog-extended/archive/refs/tags/v1.2.1.zip
    )
    FetchContent_GetProperties(nativefiledialog-extended)
    if (NOT nativefiledialog-extended_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(nativefiledialog-extended)
        add_subdirectory(${nativefiledialog-extended_SOURCE_DIR} ${nativefiledialog-extended_BINARY_DIR})
        
    endif()
endif()

set(NATIVEFILEDIALOG-EXTENDED_IncludeDirectories
    ${nativefiledialog-extended_SOURCE_DIR}/src/include
)