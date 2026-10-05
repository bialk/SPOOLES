# Building SPOOLES

The repository provides Ninja-based CMake presets for the MSVC and LLVM
(`clang-cl`) toolchains. Each toolchain has Debug and Release configurations.

To build, test, and install every configuration from Git Bash, run:

```bash
./scripts/build-all.sh
```

An optional first argument overrides the number of parallel jobs:

```bash
./scripts/build-all.sh 16
```

The equivalent PowerShell entry point remains available:

```powershell
.\scripts\build-all.ps1
```

The script initializes the Visual Studio build environment automatically and
uses all available logical processors. Build trees and installation prefixes
are kept outside the source tree under the sibling directory
`spooles-install-std-thread`:

```text
spooles-install-std-thread/
  build/<preset>/
  install/msvc/
  install/llvm/
```

Each compiler prefix contains `lib/spooles.lib` for Release and
`lib/spoolesd.lib` for Debug. The installed `SPOOLES::spooles` CMake target
selects the matching configuration.

An individual configuration can also be handled directly:

```powershell
cmake --preset msvc-debug
cmake --build --preset msvc-debug --parallel
ctest --preset msvc-debug
cmake --install ..\spooles-install-std-thread\build\msvc-debug
```

The installed CMake package exports the target `SPOOLES::spooles`:

```cmake
find_package(SPOOLES CONFIG REQUIRED)
target_link_libraries(my_target PRIVATE SPOOLES::spooles)
```

Debug libraries have the `d` suffix (`spoolesd.lib`) so they cannot be
mistaken for Release libraries. Keep each application and SPOOLES on the same
MSVC runtime configuration, especially when using Debug builds.

MSVC and `clang-cl` builds use the dynamic runtime by default: `/MDd` in Debug
and `/MD` in Release. A static runtime can be selected at
configure time with `-DSPOOLES_MSVC_RUNTIME=static`, producing `/MTd` or `/MT`
instead. The application linking SPOOLES should use the same runtime family.
