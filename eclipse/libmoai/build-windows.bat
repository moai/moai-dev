@echo off

pushd libcrypto
call build.bat false
popd

bash build-windows.sh
if not "%1" == "false" pause