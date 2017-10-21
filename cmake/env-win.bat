@echo off

rem ----- defaults ----

rem find sdk
if NOT "%MOAI_SDK_HOME%" == "" goto testcmake
pushd "%~dp0%..\" 
set "MOAI_SDK_HOME=%cd%"
popd

:testcmake
where cmake
if ERRORLEVEL 1 (
  if EXIST "%ProgramFiles(x86)%\CMake\bin" set "CMAKE_PATH=%ProgramFiles(x86)%\CMake\bin"
)

:envlocal
if exist "%~dp0%\env-local.bat" (
  call "%~dp0%\env-local.bat"
)

rem ---- android NDK -------
:ndk
if "%NDK_PATH%"=="" goto :mingw
echo "Setting Android NDK path..."

set "ANDROID_NDK=%NDK_PATH%"


rem ---- moai util path -----
:util
echo "Setting Moai Util path..."

pushd .
cd "%~dp0%/.."
set "UTIL_PATH=%cd%\bin"
popd

set "PATH=%PATH%;%UTIL_PATH%"

rem ---- Doxygen -----
if "%DOXYGEN_PATH%"=="" goto :end
echo "Setting DOXYGEN path..."
set "PATH=%PATH%;%DOXYGEN_PATH%;%DOT_PATH%"

:end
echo "Path setup complete"