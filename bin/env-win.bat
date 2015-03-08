@echo off

rem ---- SETTINGS -------
rem * edit or remove these to suit your setup

set CMAKE_PATH=K:\dev\mobile\moaiforge\cmake\bin
set NDK_PATH=K:\dev\mobile\moaiforge\android-ndk-r9d
set MINGW_PATH=K:\dev\mobile\moaiforge\mingw64\bin
set EMSDK_PATH=K:\dev\mobile\moaiforge\emsdk-1.25
set JAVA_HOME=C:\Program Files\Java\jdk1.7.0_25

rem --OPTIONAL for docs--

set DOXYGEN_PATH=K:\dev\mobile\doxygen
set DOT_PATH=K:\dev\mobile\graphviz\bin

rem ----------------------



rem ---- cmake ------
echo "Setting CMAKE bin path..."

set PATH=%PATH%;%CMAKE_PATH%



rem ---- visual studio ----
:vstudio
if "%VS120COMNTOOLS%"=="" echo Visual Studio not found..SKIPPED && goto :ndk 
echo "Setting Visual Studio path..."

pushd .
call "%VS120COMNTOOLS%\VsDevCmd.bat"
popd



rem ---- android NDK -------
:ndk
if "%NDK_PATH%"=="" goto :mingw
echo "Setting Android NDK path..."

set ANDROID_NDK=%NDK_PATH%



rem ----- mingw ----------
:mingw
if "%MINGW_PATH%"=="" goto :emsdk
echo "Setting MingW Gcc path..."

set PATH=%PATH%;%MINGW_PATH%



rem ---- emscripten SDK -------
:emsdk
if "%EMSDK_PATH%"=="" goto :util
echo "Setting Emscripten path..."

pushd .
cd %EMSDK_PATH%
call emsdk_env.bat
popd



rem ---- moai util path -----
:util
echo "Setting Moai Util path..."

pushd .
cd %~dp0%/..
set UTIL_PATH=%cd%/util
popd

set PATH=%PATH%;%UTIL_PATH%

rem ---- Doxygen -----
if "%DOXYGEN_PATH%"=="" goto :end
echo "Setting DOXYGEN path..."
set PATH=%PATH%;%DOXYGEN_PATH%;%DOT_PATH%

:end
echo "Path setup complete"