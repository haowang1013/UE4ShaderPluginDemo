@echo off
pushd %~dp0

CALL .\FindEnginePath.cmd

:PATH_FOUND
set "SUB_PATH=Engine\Binaries\Win64\UE4Editor.exe"
set CURRENT_PATH=%~dp0%PROJECT_DIR%
set ARGUMENTS=%*
cd %PROJECT_DIR%
start %ENGINE_PATH%\%SUB_PATH% "%CURRENT_PATH%%UPROJECT_FILE%" %ARGUMENTS%

popd
