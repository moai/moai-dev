@echo off
setlocal enableextensions

rem Requirements Check

if "%ANDROID_NDK%"=="" echo "You need to set ANDROID_NDK to your ndk path" && exit /b 1



cd %~dp0%..
set MOAIROOT=%cd%

set defaultprefix=%MOAIROOT%\lib\android
set libprefix=%1
if "%libprefix%"=="" set libprefix=%defaultprefix%

set build_folder="%MOAIROOT%\build"
mkdir %build_folder%

for %%G in (armeabi,armeabi-v7a,x86) DO (
cd %build_folder%
mkdir build-android-%%G
cd build-android-%%G


cmake ^
-DBUILD_ANDROID=TRUE ^
-DCMAKE_TOOLCHAIN_FILE="%MOAIROOT%\cmake\hosts\host-android\android.toolchain.cmake" ^
-DCMAKE_BUILD_TYPE=Release ^
-DMOAI_LUAJIT=False ^
-DANDROID_ABI=%%G ^
-DCMAKE_INSTALL_PREFIX=%libprefix%\%%G ^
-DLIBRARY_OUTPUT_PATH_ROOT=.\build-android-%%G\ ^
-G"MinGW Makefiles" ^
-DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%\prebuilt\windows\bin\make.exe" ^
%MOAIROOT%\cmake || exit /b 1

cmake --build . --target install || exit /b 1
)

