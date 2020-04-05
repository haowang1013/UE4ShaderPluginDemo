@ECHO OFF

pushd %~dp0

CALL .\FindEnginePath.cmd

SET WITH_VS_2017=
SET BUILD_SHADER_COMPILER=1
SET BUILD_FRONTEND=1
SET BUILD_LIGHTMASS=1
SET BUILD_TYPE=Development

IF "%IS_INSTALLED_BUILD%"=="1" (
	SET BUILD_SHADER_COMPILER=0
	SET BUILD_FRONTEND=0
	SET BUILD_LIGHTMASS=0
)

:loop
IF NOT "%1"=="" (
    IF "%1"=="-2017" (
		SET WITH_VS_2017=-2017
		GOTO :PARSED
	)
    IF "%1"=="--no-shader-compiler" (
		SET BUILD_SHADER_COMPILER=0
		GOTO :PARSED
	)
	IF "%1"=="--no-frontend" (
		SET BUILD_FRONTEND=0
		GOTO :PARSED
	)
	IF "%1"=="--no-lightmass" (
		SET BUILD_LIGHTMASS=0
		GOTO :PARSED
	)
	IF "%1"=="--debug-game" (
		SET BUILD_TYPE=DebugGame
		GOTO :PARSED
	)
	
	echo invalid arument: %1
	echo valid arguments:
	echo -2017: use visual studio 2017
	echo --no-shader-compiler: skip building shader compiler
	echo --no-frontend: skip building frontend
	echo --no-lightmass: skip building lightmass
	echo --debug-game: build game with DebugGame instead of Development
	GOTO :eof
	
:PARSED
    SHIFT
    GOTO :loop
)

cd %PROJECT_DIR%

rem set build.version to readonly if it is not already
IF "%IS_INSTALLED_BUILD%"=="1" (
	attrib +R %ENGINE_PATH%\Engine\Binaries\Win64\UE4Editor.version
)

if %BUILD_SHADER_COMPILER%==1 call %ENGINE_PATH%\Engine\Build\BatchFiles\Build.bat ShaderCompileWorker Win64 Development -waitmutex %WITH_VS_2017%
if %BUILD_FRONTEND%==1 call %ENGINE_PATH%\Engine\Build\BatchFiles\Build.bat UnrealFrontend Win64 Development -waitmutex %WITH_VS_2017%
if %BUILD_LIGHTMASS%==1 call %ENGINE_PATH%\Engine\Build\BatchFiles\Build.bat UnrealLightmass Win64 Development -waitmutex %WITH_VS_2017%
call %ENGINE_PATH%\Engine\Build\BatchFiles\Build.bat %PROJECT_NAME%Editor Win64 %BUILD_TYPE% %cd%\%UPROJECT_FILE% -waitmutex %WITH_VS_2017%

popd

pause
