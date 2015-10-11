@echo off

if exist "%~dp0%\env-local.bat" (
  call "%~dp0%\env-local.bat"
  ) else (
  echo "Couldn't find local settings file env-local.bat"
  echo "Please copy env-local.bat.template to env-local.bat and rerun."
  exit /b 1
)


rem ---- cmake ------
echo "Setting CMAKE bin path..."

if "%CMAKE_PATH%"=="" goto :vstudio
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