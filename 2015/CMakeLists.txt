cmake_minimum_required(VERSION 3.25)

project("advent-of-code-2015-c"
    VERSION 0.0.1
    LANGUAGES CXX C
)

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
