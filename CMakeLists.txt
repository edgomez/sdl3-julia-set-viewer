cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(SDL3JuliaSetViewer
    LANGUAGES CXX
)

if(EXISTS "${CMAKE_SOURCE_DIR}/external/SDL")
    add_subdirectory("${CMAKE_SOURCE_DIR}/external/SDL")
else()
    find_package(SDL3 REQUIRED)
endif()

add_executable(sdl3-julia-set-viewer
    src/main.cpp
)
target_compile_options(sdl3-julia-set-viewer
    PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive->"
        "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>"
)
target_link_libraries(sdl3-julia-set-viewer
    PRIVATE
        SDL3::SDL3
)