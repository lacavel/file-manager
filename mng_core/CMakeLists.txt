add_library(mng_core STATIC)

target_compile_features(mng_core PRIVATE c_std_17)
target_sources(mng_core PRIVATE src/ent.c src/ent_sts.c src/ent_sts_cache.c)
target_include_directories(
    mng_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/int
)
