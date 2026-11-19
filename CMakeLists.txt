cmake_minimum_required(VERSION 4.4.2)
project(file-manager VERSION 1.0.0 LANGUAGES C CXX)

add_compile_options(-Wall -Wextra -g)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Werror)
    add_compile_options(-Og)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
else()
    add_compile_options(-O3)
endif()

option(ENABLE_COMP_MODE "Disable Nerd Font icons." OFF)

if(ENABLE_COMP_MODE)
    add_compile_definitions(COMP_MODE=1)
else()
    add_compile_definitions(COMP_MODE=0)
endif()

add_subdirectory(mng_core)
add_subdirectory(mng)
