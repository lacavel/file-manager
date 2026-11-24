cmake_minimum_required(VERSION 4.4.2)
project(file-manager VERSION 1.0.0 LANGUAGES C CXX)

option(ENABLE_COMP_MODE "Disable Nerd Font icons." OFF)

if(ENABLE_COMP_MODE)
    add_compile_definitions(COMP_MODE=1)
else()
    add_compile_definitions(COMP_MODE=0)
endif()

include(cmake/c.cmake)
include(cmake/deps/ncurses.cmake)

add_subdirectory(mng_core)
add_subdirectory(mng_gfx)
add_subdirectory(mng)
