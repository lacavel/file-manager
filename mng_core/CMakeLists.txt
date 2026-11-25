add_library(mng_core STATIC)

target_compile_features(mng_core PUBLIC cxx_std_20)
target_sources(
    mng_core
    PRIVATE src/dir_qry.cpp src/ent_qry.cpp src/ent_sys_qry.cpp src/id_qry.cpp
)
target_include_directories(
    mng_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/int
)
target_link_libraries(mng_core PRIVATE strict_c)
