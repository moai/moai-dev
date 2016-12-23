@echo off

:: Remember the original working directory
pushd .

:: Move to cmake directory
cd "%~dp0%..\cmake"

:: Determine target directory and cmake generator
set arg1=%1
if "%arg1%"=="" set arg1=vs2012
set generator=
if "%arg1%"=="vs2008" set generator=Visual Studio 9 2008
if "%arg1%"=="vs2010" set generator=Visual Studio 10
if "%arg1%"=="vs2012" set generator=Visual Studio 11
if "%arg1%"=="vs2013" set generator=Visual Studio 12
if "%generator%"=="" (
	@echo Unknown argument "%1". Valid values are vs2008, vs2010, vs2012, vs2013. Exiting.
	goto end
)
set targetDir=projects\%arg1%

@echo on

:: Delete and re-create the target directory
if exist %targetDir% rmdir /s /q %targetDir%
md %targetDir%

:: Move to target directory, then call cmake from there
cd %targetDir%
cmake ^
-G "%generator%" ^
-DBUILD_WINDOWS=TRUE ^
-DMOAI_SDL=TRUE ^
..\..\hosts\host-win-sdl\

:end

@echo off

:: Restore original working directory
popd
