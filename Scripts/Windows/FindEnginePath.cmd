@echo off

rem Find engine path relative to the game root directory (NOT relative to the script folder this file is at)

set PROJECT_DIR=..\..\
if "%1"=="projdir-only" goto :eof

pushd %~dp0%PROJECT_DIR%

set UPROJECT_FILE=
set PROJECT_NAME=
set IS_INSTALLED_BUILD=
for /f %%i in ('dir /b *.uproject') do (
		set UPROJECT_FILE=%%i
		set PROJECT_NAME=%%~ni
	)

set ENGINE_ASSOCIATION=
for /f "tokens=2 delims=:" %%i in ('FINDSTR /C:"EngineAssociation" %UPROJECT_FILE%') do (
	set LINE=%%i
)

for /f "tokens=*" %%i in (%LINE%) do set ENGINE_ASSOCIATION=%%i

set ENGINE_PATH=
IF EXIST %ENGINE_ASSOCIATION% (
	set ENGINE_PATH=%ENGINE_ASSOCIATION%
	GOTO PATH_FOUND
)

REM Get UE4 engine path, magic
set UE4_REGKEY="HKEY_CURRENT_USER\Software\Epic Games\Unreal Engine\Builds"
set UE4_REGVAL=%ENGINE_ASSOCIATION%

REM Check for presence of key first.
reg query %UE4_REGKEY% /v %UE4_REGVAL% 2>nul || (echo No ue4 engine name present! & exit /b 1)

REM query the value. pipe it through findstr in order to find the matching line that has the value. only grab token 3 and the remainder of the line. %%b is what we are interested in here.
set UE4_NAME=
for /f "tokens=2,*" %%a in ('reg query %UE4_REGKEY% /v %UE4_REGVAL% ^| findstr %UE4_REGVAL%') do (
    set UE4_NAME=%%b
)

REM Possibly no value set
if not defined UE4_NAME (echo No ue4 engine name present! & exit /b 1)

REM replace any spaces with +
set UE4_NAME=%UE4_NAME: =+%

IF EXIST %UE4_NAME% (
	set ENGINE_PATH=%UE4_NAME%
	GOTO PATH_FOUND
)

echo "Associated engine cannot be found!"
pause
exit /b 1

:PATH_FOUND
set ENGINE_PATH=%ENGINE_PATH:/=\%
IF EXIST "%ENGINE_PATH%\Engine\Build\InstalledBuild.txt" (
	set IS_INSTALLED_BUILD=1
)
popd

