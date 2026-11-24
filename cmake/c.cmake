include_guard(GLOBAL)

option(
    ENABLE_STRICT_COMPILATION
    "Treat compiler warnings as errors in Debug builds."
    ON
)

option(ENABLE_SANITIZERS "Enable sanitizers in Debug builds." ON)

add_library(mng_c INTERFACE)

if(
    CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$"
    AND CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$"
)
    target_compile_options(
        mng_c
        INTERFACE -Wall -Wextra -Wpedantic $<$<CONFIG:Debug>:-Og>
    )

    if(ENABLE_STRICT_COMPILATION)
        target_compile_options(mng_c INTERFACE $<$<CONFIG:Debug>:-Werror>)
    endif()

    if(ENABLE_SANITIZERS)
        target_compile_options(
            mng_c
            INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined>
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        target_link_options(
            mng_c
            INTERFACE $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        )
    endif()
endif()
