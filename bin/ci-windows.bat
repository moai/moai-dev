@echo off

echo "Setting Moai Util path..."

pushd .
cd %~dp0%/..
set UTIL_PATH=%cd%/util

set PATH=%PATH%;%UTIL_PATH%


echo Compiling Windows Libs
call bin\build-windows.bat

echo Creating Windows Host
mkdir hosttest
cd hosttest
moaiutil host init
moaiutil host build windows

popd