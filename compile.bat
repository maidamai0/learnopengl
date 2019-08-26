:: compile for windows
@echo off

set ROOT_DIR=%~dp0
set BUILD_DIR=%ROOT_DIR%build\
set PROJ_FILE=%BUILD_DIR%learn_opengl.sln

cd %BUILD_DIR%
cmake -G "Visual Studio 16 2019" -A x64 ..
cd ..
msbuild %PROJ_FILE% /m:20 /v:m /ignore:4099