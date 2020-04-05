@echo OFF
pushd ..

call .\FindEnginePath.cmd

pushd %PROJECT_DIR%
set PROJECT_ABS_PATH=%CD%\%UPROJECT_FILE%
set ArchiveDirectory=%CD%\PackagedGame
popd

set ARGUMENTS=%*

call %PROJECT_DIR%\%ENGINE_PATH%\Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project=%PROJECT_ABS_PATH% -noP4 -Platform=%Platform% -CookFlavor=%CookFlavor% -ClientConfig=%ClientConfig% -Cook -Package -Compressed -UnversionedCookedContent -NoCompileEditor -build -stage -pak -archive -ArchiveDirectory=%ArchiveDirectory% %ARGUMENTS%

if not %Platform% == IOS explorer %ArchiveDirectory%

popd

popd
