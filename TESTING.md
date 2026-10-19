# Test Replays

The GeneralsReplays folder contains replays and the required maps that are tested in CI to ensure that the game is retail compatible.

You can also test with these replays locally:
- Copy the replays into a subfolder in your `%USERPROFILE%/Documents/Command and Conquer Generals Zero Hour Data/Replays` folder.
- Copy the maps into `%USERPROFILE%/Documents/Command and Conquer Generals Zero Hour Data/Maps`
- Start the test with this: (copy into a .bat file next to your executable)
```
START /B /W generalszh-custom.exe -jobs 4 -headless -replay subfolder/*.rep > replay_check.log
echo %errorlevel%
PAUSE
```
It will run the game in the background and check that each replay is compatible. You need to use a VC6 build with optimizations and RTS_BUILD_OPTION_DEBUG = OFF, otherwise the game won't be compatible.

## Custom build and fullscreen smoke test

This fork's default Zero Hour output is `generalszh-custom.exe`. Reuse that name
for family-play builds instead of creating a separate executable name per fix.
Leave `RTS_BUILD_OUTPUT_SUFFIX` empty for these builds. The existing
`ZEROHOUR_GAME_DIR` post-build copy uses the target filename automatically and
does not overwrite the retail executable. All family players should use the
same build; a successful graphics smoke test does not establish multiplayer or
retail-replay compatibility.

32-bit fullscreen rendering now prefers `D3DFMT_X8R8G8B8` when both the device
and a matching depth/stencil format support it. This avoids repeated device loss
seen with the advertised `D3DFMT_A8R8G8B8` fullscreen path. Windowed and 16-bit
paths are unchanged, and unsupported XRGB devices retain the existing selection.
No command-line option or environment variable is needed to enable this fix.

For diagnostics only, set `GENZH_FULLSCREEN_DIAG=1` in the game's process
environment. Each run writes a unique `FullscreenDiagnostic-<PID>-<tick>.log`
in its working directory. Check for fullscreen `format=22 windowed=0`, successful
Presents, and no repeated `Reset_Device` entries while the game has focus. Test
movies, the animated menu, and a local match, including translucent effects and
water: backbuffer destination alpha is no longer available, but texture alpha
is unchanged. Windowed mode should also be checked for regressions.

Use the normal frame cap for play. A Debug-only `-fps 60` override can measure
performance headroom, but can affect simulation speed in some modes. Keep
resolution overrides separate from the rendering fix when synchronized settings
request a mode unsupported by the current monitor.
