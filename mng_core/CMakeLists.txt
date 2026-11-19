add_library(mng_core STATIC)

target_compile_features(mng_core PRIVATE cxx_std_20)
target_sources(
    mng_core
    PRIVATE
        src/dir_qry.cpp
        src/ent_perms_qry.cpp
        src/ent_sz_qry.cpp
        src/ent_ts_qry.cpp
        src/ent_sys_qry.cpp
)
target_include_directories(
    mng_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/int
)
