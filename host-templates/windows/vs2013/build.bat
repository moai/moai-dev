@echo off
set THISPATH=%~dp0
msbuild
pushd %THISPATH%
set LUAPATH=%cd%\..\..\src
echo LUAPATH equal %LUAPATH%
mkdir bundle
cd bundle
robocopy %LUAPATH% . /MIR /XO /XF Moai.exe
xcopy /Y /D ..\Debug\moai.exe .
popd