:: compile for windows
@echo off

:: get dev environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"

md build 2> nul

set ROOT_DIR=%~dp0
set BUILD_DIR=%ROOT_DIR%build\
set PROJ_FILE=%BUILD_DIR%learn_opengl.sln

cd %BUILD_DIR%
cmake -G "Visual Studio 16 2019" -A x64 ..
cd ..
msbuild %PROJ_FILE% /m:20 /v:m /ignore:4099