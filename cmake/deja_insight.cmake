# DejaInsight profiling library (pre-built Win32/x86 static binaries)
# Provides DEJA_CONTEXT and related scoped profiling macros.
# Only available on 32-bit Windows builds.

set(_deja_dir "${CMAKE_SOURCE_DIR}/Dependencies/DejaInsight")

add_library(deja_insight STATIC IMPORTED GLOBAL)

set_target_properties(deja_insight PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_deja_dir}"

    # Debug build: static lib compiled with /MTd
    IMPORTED_LOCATION_DEBUG             "${_deja_dir}/lib/x86/MTd/DejaInsight.x86.lib"

    # Release / RelWithDebInfo / MinSizeRel: static lib compiled with /MT
    IMPORTED_LOCATION_RELEASE           "${_deja_dir}/lib/x86/MT/DejaInsight.x86.lib"
    IMPORTED_LOCATION_RELWITHDEBINFO    "${_deja_dir}/lib/x86/MT/DejaInsight.x86.lib"
    IMPORTED_LOCATION_MINSIZEREL        "${_deja_dir}/lib/x86/MT/DejaInsight.x86.lib"

    # LNK4217: dllimport decoration on static symbol (header uses _DLL guard); harmless, ignored.
    # Applied unconditionally across all configs via AdditionalOptions.
    INTERFACE_LINK_OPTIONS "/ignore:4217"
)
