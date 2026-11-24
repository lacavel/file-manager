include_guard(GLOBAL)

option(MNG_USE_SYSTEM_NCURSES "Prefer system-installed ncurses" ON)

add_library(ncurses::ncurses INTERFACE IMPORTED GLOBAL)

if(MNG_USE_SYSTEM_NCURSES)
    set(CURSES_NEED_NCURSES TRUE)
    set(CURSES_NEED_WIDE TRUE)

    find_package(Curses QUIET)

    if(CURSES_FOUND)
        find_library(NCURSES_PANEL_LIBRARY NAMES panelw)

        if(NCURSES_PANEL_LIBRARY)
            message(STATUS "Using system ncurses")

            target_include_directories(
                ncurses::ncurses
                INTERFACE ${CURSES_INCLUDE_DIRS}
            )

            target_link_libraries(
                ncurses::ncurses
                INTERFACE ${NCURSES_PANEL_LIBRARY} ${CURSES_LIBRARIES}
            )

            return()
        endif()
    endif()
endif()

message(STATUS "Building ncurses from source")

include(ExternalProject)

find_program(NCURSES_MAKE NAMES gmake make REQUIRED)

set(NCURSES_VERSION 6.5)
set(NCURSES_PREFIX "${CMAKE_BINARY_DIR}/_deps/ncurses")
set(NCURSES_INCLUDE_DIR "${NCURSES_PREFIX}/include/ncursesw")
set(NCURSES_LIBRARY "${NCURSES_PREFIX}/lib/libncursesw.a")
set(NCURSES_PANEL_LIBRARY "${NCURSES_PREFIX}/lib/libpanelw.a")

file(MAKE_DIRECTORY "${NCURSES_INCLUDE_DIR}")

ExternalProject_Add(
    ncurses_ext
    URL "https://ftp.gnu.org/gnu/ncurses/ncurses-${NCURSES_VERSION}.tar.gz"
    PREFIX "${CMAKE_BINARY_DIR}/_deps/ncurses-build"
    INSTALL_DIR "${NCURSES_PREFIX}"
    CONFIGURE_COMMAND
        <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --libdir=<INSTALL_DIR>/lib
        --enable-widec --with-normal --without-shared --without-debug
        --without-ada --without-cxx-binding --without-termlib --without-tests
        --without-manpages
    BUILD_COMMAND ${NCURSES_MAKE}
    INSTALL_COMMAND ${NCURSES_MAKE} install
    INSTALL_BYPRODUCTS "${NCURSES_LIBRARY}" "${NCURSES_PANEL_LIBRARY}"
)

add_dependencies(ncurses::ncurses ncurses_ext)

target_include_directories(ncurses::ncurses INTERFACE "${NCURSES_INCLUDE_DIR}")

target_link_libraries(
    ncurses::ncurses
    INTERFACE "${NCURSES_PANEL_LIBRARY}" "${NCURSES_LIBRARY}"
)
