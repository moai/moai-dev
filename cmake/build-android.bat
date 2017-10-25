@echo off
setlocal enableextensions
call "%~dp0%\env-win.bat"

echo "Setting CMAKE bin path..."
set "PATH=%PATH%;%CMAKE_PATH%"

echo "Setting MingW Gcc path"
set "PATH=%MINGW_PATH%;%PATH%"

rem ----- Requirements Check -----
set "PITO_HOME=%~dp0..\"

if "%ANDROID_NDK%"=="" echo "You need to set ANDROID_NDK to your ndk path try using env-win.bat and env-local.bat in scripts" && exit /b 1
if "%MOAI_SDK_HOME%"=="" echo "You need to set MOAI_SDK_HOME to your moai sdk folder" && exit /b 1

where mingw32-make || echo "mingw32-make is required. Install TCC Mingw from http://tdm-gcc.tdragon.net/ and add to the path" && goto ERROR

where cmake || echo "Cmake 3.2+ is required, download from cmake.org" && goto ERROR

rem ----- Build libmoai -----
pushd "%~dp0%"

set "CMAKEROOT=%cd%"

mkdir build
cd build
mkdir build-android
cd build-android

set "BUILDROOT=%cd%"
rem for %%a in (x86 armeabi-v7a arm64-v8a)
for %%a in (x86) do (
    cmake ^
    -H%CMAKEROOT%\cmake\hosts\host-android ^
    -B%BUILDROOT% ^
    -G"MinGW Makefiles" ^
    -DANDROID_ABI=%%a ^
    -DANDROID_NDK=%ANDROID_NDK% ^
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="%CMAKEROOT%\lib\android\%%a" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
    -DANDROID_PLATFORM=android-17 ^
    -DBUILD_ANDROID=TRUE ^
    -DMOAI_ANDROID=TRUE ^
    -DANDROID_STL=c++_static ^
    -DMOAI_SDK_HOME=%MOAI_SDK_HOME% ^
    -DHOST_ROOT=%MOAI_SDK_HOME%/src || goto ERROR
    
    cmake --build  . --target moai -- -j3 || goto ERROR
)

goto END
:ERROR
exit /b 1

:END