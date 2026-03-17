add_executable(z_generals WIN32)

# Use a binary name that doesn't conflict with original game.
if("${CMAKE_SYSTEM}" MATCHES "Windows")
    set_target_properties(z_generals PROPERTIES OUTPUT_NAME "generalszh${RTS_BUILD_OUTPUT_SUFFIX}")
else()
    set_target_properties(z_generals PROPERTIES OUTPUT_NAME "generalszh${RTS_BUILD_OUTPUT_SUFFIX}")
endif()

target_link_libraries(z_generals PRIVATE
    binkstub
    comctl32
    core_debug
    core_profile
    d3d8
    d3dx8
    dinput8
    dxguid
    imm32
    milesstub
    vfw32
    winmm
    z_gameengine
    z_gameenginedevice
    zi_always
)

# TODO Originally referred to build host and user, replace with git info perhaps?
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/GeneratedVersion.h
"#pragma once

#define VERSION_LOCALBUILDNUM 0
#define VERSION_BUILDUSER \"\"
#define VERSION_BUILDLOC \"\"
"
)

# Based on original binary values for these variables.
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/BuildVersion.h
"#pragma once

#define VERSION_MAJOR 1
#define VERSION_MINOR 4
#define VERSION_BUILDNUM 601
"
)

if(MSVC)
    target_link_options(z_generals PRIVATE "/NODEFAULTLIB:libci.lib")
endif()

target_include_directories(z_generals PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_sources(z_generals PRIVATE
    WinMain.cpp
    WinMain.h
)

# RC files optional for MinGW builds
# Icon: LoadIcon() handles missing resource gracefully (uses default system icon)
# Manifest: DPI awareness metadata (nice to have but not essential)
if(MSVC)
    # VS2005 and later adds default manifest, we need to turn it off to prevent conflict with custom manifest
    if(NOT IS_VS6_BUILD)
        target_link_options(z_generals PRIVATE
            "/MANIFEST:NO"
        )
    endif()

    target_sources(z_generals PRIVATE
        RTS.RC
    )
endif()

# Strip debug symbols to separate file for MinGW Release builds
# This creates generalszh${RTS_BUILD_OUTPUT_SUFFIX}.exe.debug (similar to MSVC .pdb files)
if(MINGW AND COMMAND add_debug_strip_target)
    add_debug_strip_target(z_generals)
endif()

# DejaInsight static libs (MT/MTd) conflict with the game's /MD runtime.
# Exclude the MT CRT default libs to suppress LNK4098.
if(TARGET deja_insight)
    target_link_options(z_generals PRIVATE
        $<$<CONFIG:Debug>:/NODEFAULTLIB:LIBCMTD>
        $<$<NOT:$<CONFIG:Debug>>:/NODEFAULTLIB:LIBCMT>
    )
endif()

# Post-build: copy exe to retail install directory for quick iteration.
# Set ZEROHOUR_GAME_DIR to the root of your Zero Hour installation to enable this.
set(_post_build_script "${CMAKE_CURRENT_BINARY_DIR}/post_build_copy.cmake")
file(WRITE "${_post_build_script}" [=[
if(DEFINED ENV{ZEROHOUR_GAME_DIR})
    cmake_path(CONVERT "$ENV{ZEROHOUR_GAME_DIR}" TO_CMAKE_PATH_LIST _dest NORMALIZE)
    get_filename_component(_name "${EXE}" NAME)
    file(COPY_FILE "${EXE}" "${_dest}/${_name}" ONLY_IF_DIFFERENT)
    message(STATUS "[z_generals] Copied ${EXE} -> ${_dest}/${_name}")
else()
    message(STATUS "[z_generals] Tip: Set the ZEROHOUR_GAME_DIR environment variable to the root of your Zero Hour installation directory to auto-copy the executable after each build.")
endif()
]=])
add_custom_command(TARGET z_generals POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DEXE=$<TARGET_FILE:z_generals> -P "${_post_build_script}"
    VERBATIM
)
