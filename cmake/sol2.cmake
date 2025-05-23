include(FetchContent)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(sol2)


set(sol2_IncludeDirectories
    ${sol2_SOURCE_DIR}/include
)