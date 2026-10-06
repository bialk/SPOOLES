[CmdletBinding()]
param(
    [ValidateRange(1, 1024)]
    [int]$Jobs = [Environment]::ProcessorCount,
    [switch]$MPI
)

$ErrorActionPreference = 'Stop'
$sourceDirectory = Split-Path -Parent $PSScriptRoot
$commonDirectory = Join-Path (Split-Path -Parent $sourceDirectory) 'spooles-install-std-thread'

function Invoke-CMake {
    param([Parameter(Mandatory)][string[]]$Arguments)

    & cmake @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "cmake $($Arguments -join ' ') failed with exit code $LASTEXITCODE"
    }
}

function Invoke-CTest {
    param([Parameter(Mandatory)][string[]]$Arguments)

    & ctest @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "ctest $($Arguments -join ' ') failed with exit code $LASTEXITCODE"
    }
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw 'Visual Studio Installer vswhere.exe was not found.'
}

$visualStudioDirectory = & $vswhere -latest -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath
if (-not $visualStudioDirectory) {
    throw 'Visual Studio C++ Build Tools were not found.'
}

$developerCommand = Join-Path $visualStudioDirectory 'Common7\Tools\VsDevCmd.bat'
$developerEnvironment = & cmd.exe /d /s /c `
    "call `"$developerCommand`" -arch=x64 -host_arch=x64 >nul && set"
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to initialize the Visual Studio x64 developer environment.'
}

$pathEntries = @($developerEnvironment |
    Where-Object { $_ -match '^Path=' })
$developerPathEntry = $pathEntries |
    Where-Object { $_ -match '\\VC\\Tools\\MSVC\\' } |
    Select-Object -First 1
if (-not $developerPathEntry) {
    $developerPathEntry = $pathEntries | Select-Object -First 1
}
if (-not $developerPathEntry) {
    throw 'The Visual Studio developer PATH was not returned by VsDevCmd.'
}

foreach ($entry in $developerEnvironment) {
    $separator = $entry.IndexOf('=')
    if ($separator -gt 0) {
        $name = $entry.Substring(0, $separator)
        $value = $entry.Substring($separator + 1)
        if ($name -ine 'Path') {
            [Environment]::SetEnvironmentVariable($name, $value, 'Process')
        }
    }
}

$developerPath = $developerPathEntry.Substring($developerPathEntry.IndexOf('=') + 1)
$env:Path = $developerPath

if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    throw 'VsDevCmd completed, but cl.exe is not available on PATH.'
}

$clangCompiler = 'C:\Program Files\LLVM\bin\clang-cl.exe'
if (-not (Test-Path -LiteralPath $clangCompiler)) {
    throw "LLVM compiler was not found at $clangCompiler"
}

if ($MPI) {
    $presets = @('msvc-mpi-debug', 'msvc-mpi-release',
                 'llvm-mpi-debug', 'llvm-mpi-release')
} else {
    $presets = @('msvc-debug', 'msvc-release',
                 'llvm-debug', 'llvm-release')
}

Push-Location $sourceDirectory
try {
    foreach ($preset in $presets) {
        Write-Host "`n=== Configuring $preset ==="
        Invoke-CMake @('--preset', $preset, '--fresh')

        Write-Host "`n=== Building $preset with $Jobs jobs ==="
        Invoke-CMake @('--build', '--preset', $preset, '--parallel', $Jobs)

        Write-Host "`n=== Testing $preset ==="
        Invoke-CTest @('--preset', $preset, '--parallel', $Jobs)

        Write-Host "`n=== Installing $preset ==="
        $buildDirectory = Join-Path $commonDirectory "build\$preset"
        Invoke-CMake @('--install', $buildDirectory)
    }
}
finally {
    Pop-Location
}

Write-Host "`nAll SPOOLES configurations built, tested, and installed under:"
Write-Host $commonDirectory
