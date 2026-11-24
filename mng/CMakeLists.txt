add_executable(mng)

target_compile_features(mng PRIVATE c_std_17 cxx_std_20)
target_sources(mng PRIVATE src/main.cpp src/args.c)

target_include_directories(
    mng
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/int
)
target_link_libraries(mng PRIVATE mng_core mng_gfx mng_c)
