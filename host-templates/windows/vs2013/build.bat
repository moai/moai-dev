@echo off
set THISPATH=%~dp0

REM check for visual studio
if not "%VS120COMNTOOLS%"=="" goto :build
echo Build script could not find the VS120COMNTOOLS env var. Do you have vs2013 installed?
echo You can try to run the command again from a visual studio developer command prompt.
exit 1

:build
call "%VS120COMNTOOLS%\VsDevCmd.bat"
msbuild
pushd %THISPATH%
set LUAPATH=%cd%\..\..\src
echo LUAPATH equal %LUAPATH%
mkdir bundle
cd bundle
robocopy %LUAPATH% . /MIR /XO /XF Moai.exe
xcopy /Y /D ..\Debug\moai.exe .
popd