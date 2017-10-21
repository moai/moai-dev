@echo off
setlocal enableextensions
call "%~dp0%\env-win.bat"
rem ----- Requirements Check -----
set "PITO_HOME=%~dp0..\"

if "%ANDROID_NDK%"=="" echo "You need to set ANDROID_NDK to your ndk path try using env-win.bat and env-local.bat in scripts" && exit /b 1
if "%MOAI_SDK_HOME%"=="" echo "You need to set MOAI_SDK_HOME to your moai sdk folder" && exit /b 1

rem ----- Build libmoai using sdk methods -----

pushd "%MOAI_SDK_HOME%\ant"

if not exist libmoai\jni\libraries.mk call libmoai-clean.bat

if "%1"=="--clean" call libmoai-clean.bat

echo "Builing libmoai"
call libmoai-build.bat || echo "Android NDK build failed" && exit /b 1

mkdir "%PITO_HOME%\lib\android\libs"
mkdir "%PITO_HOME%\lib\android\obj"

robocopy libmoai\libs "%PITO_HOME%\lib\android\libs" /mir /NDL /NJH /NJS 
robocopy libmoai\obj "%PITO_HOME%\lib\android\obj" *.a /S /NDL  /NJH /NJS 

rem robocopy has a derpy exit code
exit /b 0