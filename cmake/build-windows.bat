@echo off
setlocal enableextensions
call "%~dp0%\env-win.bat"

echo "Setting CMAKE bin path..."
set "PATH=%PATH%;%CMAKE_PATH%"



rem Prerequisites
if "%MOAI_SDK_HOME%"=="" echo "Could not determine location of MOAI SDK, please set MOAI_SDK_HOME" && goto ERROR

where cmake || echo "Cmake 2.8.11+ is required, download from cmake.org" && goto ERROR


echo "MOAI SDK HOME: %MOAI_SDK_HOME%"
rem find and configure vs2017

set VsWhereBin="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set VSCMD_ARG_HOST_ARCH=x64
set VSCMD_ARG_TGT_ARCH=x86

if not exist %VsWhereBin% (
	echo "Couldn't find vswhere, you need visual studio 15.2 or later"
	exit /b 1
)

for /f "usebackq tokens=*" %%i in (`%VsWhereBin% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
  set InstallDir=%%i
)

if exist "%InstallDir%\VC\Auxiliary\Build\vcvarsall.bat" (
  call "%InstallDir%\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
	echo "Couldn't find VC 2017"
	exit /b 1
)

rem compile


:COMPILE

pushd "%~dp0%"

set "CMAKEROOT=%cd%"

mkdir build
cd build
mkdir build-windows
cd build-windows
cmake ^
-DMOAI_SDK_HOME="%MOAI_SDK_HOME%" ^
-DBUILD_WINDOWS=TRUE ^
-DCMAKE_BUILD_TYPE=Release ^
-DMOAI_SDK=TRUE ^
-DMOAI_LUAJIT=FALSE ^
-DCMAKE_INSTALL_PREFIX="%CMAKEROOT%\lib\windows" ^
-DCMAKE_GENERATOR_PLATFORM=Win32 ^
-G "Visual Studio 15 2017" ^
%CMAKEROOT%\cmake\hosts\host-win-sdl || goto ERROR

cmake --build  . --target moai || goto ERROR

popd
rem Install into lib

mkdir "%CMAKEROOT%\lib\windows"
echo "TODO COPY LIBS"

goto END

:ERROR
exit /b 1

:END

exit /b 0
