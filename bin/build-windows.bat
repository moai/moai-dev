@echo off
rem :: Determine target directory and cmake generator
setlocal enableextensions

where lib || echo "Could not find lib.exe (are you in your VS developer tools prompt?)" && exit /b 1

:haslib
set arg1=%1
if "%arg1%"=="" set arg1=vs2013
set generator=
if "%arg1%"=="vs2008" set generator=Visual Studio 9 2008
if "%arg1%"=="vs2010" set generator=Visual Studio 10
if "%arg1%"=="vs2012" set generator=Visual Studio 11
if "%arg1%"=="vs2013" set generator=Visual Studio 12
if "%generator%"=="" (
	@echo Unknown argument "%1". Valid values are vs2008, vs2010, vs2012, vs2013. Exiting.
	exit /b 1
)
cd %~dp0%..
set rootpath=%cd%
set defaultprefix=%rootpath%\lib\windows\%arg1%
set libprefix=%2
if "%libprefix%"=="" set libprefix=%defaultprefix%

mkdir "build\build-%arg1%"
cd "build\build-%arg1%"

echo Creating Release Libs
cmake -G "%generator%" ^
-DBUILD_WINDOWS=true ^
-DMOAI_SDL=true ^
-DMOAI_HTTP_SERVER=true ^
-DCMAKE_INSTALL_PREFIX=%libprefix%\Release ^
%rootpath%\cmake\hosts\host-win-sdl || exit /b 1

cmake --build . --target INSTALL --config Release || exit /b 1

erase  libmoai\third-party\luajit\luajit\src\lua51.lib

echo Creating Debug Libs
cmake -DCMAKE_INSTALL_PREFIX=%libprefix%\Debug %rootpath%\cmake\hosts\host-win-sdl || exit /b 1


if "%CI%"=="TRUE" goto skipdebug
cmake --build . --target INSTALL --config Debug  || exit /b 1

:skipdebug
echo Creating Distribute Libs
rmdir /S/Q %libprefix%\Distribute\lib

md %libprefix%\Distribute\lib
lib /OUT:%libprefix%\Distribute\lib\moai.LIB %libprefix%\Release\lib\*.lib || exit /b 1

lib /OUT:%libprefix%\Distribute\lib\moai_d.LIB %libprefix%\Debug\lib\*.lib || exit /b 1
xcopy /S/I/Y %libprefix%\Release\include %libprefix%\Distribute\include  || exit /b 1
mkdir %libprefix%\Distribute\bin
copy /Y %libprefix%\Release\bin\moai.exe %libprefix%\Distribute\bin\moai.exe


if NOT EXIST %rootpath%\util\moai.exe copy /Y %libprefix%\Release\bin\moai.exe %rootpath%\util\moai.exe

rd /S/Q %libprefix%\Release
rd /S/Q %libprefix%\Debug

echo "Build complete"