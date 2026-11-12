add_library(mng_core STATIC)

target_compile_features(mng_core PRIVATE c_std_17)
target_sources(
    mng_core
    PRIVATE
        src/e_qry.c
        src/e_perms_qry.c
        src/e_tm_qry.c
        src/e_sts_qry.c
        src/e_utils.c
        src/e_sz_qry.c
)
target_include_directories(
    mng_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/int
)
