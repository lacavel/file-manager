add_executable(mng)

find_package(Curses REQUIRED)

target_compile_features(mng PRIVATE c_std_17)
target_sources(mng PRIVATE src/main.c src/cntlr.c src/v.c)

target_include_directories(
    mng
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/int
    PRIVATE ${CURSES_INCLUDE_DIRS}
)
target_link_libraries(mng PRIVATE mng_core ${CURSES_LIBRARIES} panel)
