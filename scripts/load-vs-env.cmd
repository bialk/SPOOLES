@echo off
if not defined SPOOLES_VSDEVCMD exit /b 2
call "%SPOOLES_VSDEVCMD%" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b %errorlevel%
set
