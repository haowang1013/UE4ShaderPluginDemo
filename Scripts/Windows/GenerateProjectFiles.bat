pushd %~dp0
CALL .\FindEnginePath.cmd

cd %PROJECT_DIR%
IF "%IS_INSTALLED_BUILD%"=="1" (
	echo generating project files for pre-installed version of engine
	call %ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool.exe -ProjectFiles %cd%\%UPROJECT_FILE% -Game %*
	goto :COMPLETED
) 

IF EXIST "%ENGINE_PATH%\Engine\Build\BatchFiles\GenerateProjectFiles.bat" (
	echo generating project files for source code version of engine
	call %ENGINE_PATH%\Engine\Build\BatchFiles\GenerateProjectFiles.bat -project=%cd%\%UPROJECT_FILE% -Engine -Game %*
	goto :COMPLETED
)

echo Unrecognized engine - not pre-installed, GenerateProjectFiles.bat not found either

:COMPLETED
popd
pause
