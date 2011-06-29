@echo off

echo ----------------------------------------------------------------
echo Building libmoai...
echo ----------------------------------------------------------------

pushd libcrypto-windows
call build.bat false
popd

bash build-windows.sh
if not "%1" == "false" pause