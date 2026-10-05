#!/usr/bin/env bash

set -euo pipefail

jobs="${1:-$(nproc)}"
if [[ ! "$jobs" =~ ^[1-9][0-9]*$ ]]; then
    echo "Usage: $0 [positive-job-count]" >&2
    exit 2
fi

source_directory="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -W)"
common_directory="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd -W)/spooles-install-std-thread"

vswhere='/c/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe'
if [[ ! -x "$vswhere" ]]; then
    echo 'Visual Studio Installer vswhere.exe was not found.' >&2
    exit 1
fi

visual_studio_directory="$("$vswhere" -latest -products '*' \
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 \
    -property installationPath | tr -d '\r')"
if [[ -z "$visual_studio_directory" ]]; then
    echo 'Visual Studio C++ Build Tools were not found.' >&2
    exit 1
fi

developer_command="$visual_studio_directory/Common7/Tools/VsDevCmd.bat"
developer_command_windows="$(cygpath -w "$developer_command")"
export SPOOLES_VSDEVCMD="$developer_command_windows"
environment_helper="$(cygpath -w "$source_directory/scripts/load-vs-env.cmd")"

# Git Bash cannot execute a batch file directly. Import the environment that
# VsDevCmd prepares, converting its Windows PATH to Git Bash form.
while IFS='=' read -r name value; do
    value="${value%$'\r'}"
    if [[ "$name" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
        if [[ "${name,,}" == 'path' ]]; then
            export PATH="$(cygpath --path --unix "$value")"
        else
            export "$name=$value"
        fi
    fi
done < <(cmd.exe //d //c "$environment_helper")
unset SPOOLES_VSDEVCMD

if ! command -v cl.exe >/dev/null 2>&1; then
    echo 'VsDevCmd completed, but cl.exe is not available on PATH.' >&2
    exit 1
fi

presets=(
    msvc-debug
    msvc-release
    llvm-debug
    llvm-release
)

cd "$source_directory"
for preset in "${presets[@]}"; do
    printf '\n=== Configuring %s ===\n' "$preset"
    cmake --preset "$preset" --fresh

    printf '\n=== Building %s with %s jobs ===\n' "$preset" "$jobs"
    cmake --build --preset "$preset" --parallel "$jobs"

    printf '\n=== Testing %s ===\n' "$preset"
    ctest --preset "$preset" --parallel "$jobs"

    printf '\n=== Installing %s ===\n' "$preset"
    cmake --install "$common_directory/build/$preset"
done

printf '\nAll SPOOLES configurations built, tested, and installed under:\n%s\n' \
    "$common_directory"
