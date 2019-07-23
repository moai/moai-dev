@echo off
rem :: Determine target directory and cmake generator
setlocal enableextensions
call "%~dp0%\env-win.bat"

if "%MOAI_SDK_HOME%"=="" (
	echo Could not determine location of MOAI SDK, please set MOAI_SDK_HOME
	exit /b 1
)


set VsWhereBin="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set VSCMD_ARG_HOST_ARCH=x64
set VSCMD_ARG_TGT_ARCH=x86

if not exist %VsWhereBin% (
	echo "Couldn't find vswhere, you need visual studio 15.2 or later"
	exit /b 1
)

for /f "usebackq tokens=*" %%i in (`%VsWhereBin% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
  set InstallDir=%%i
)

if exist "%InstallDir%\VC\Auxiliary\Build\vcvarsall.bat" (
  call "%InstallDir%\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
	echo "Couldn't find VC 2017"
	exit /b 1
)


cd "%~dp0.."
set "rootpath=%cd%"
set "libprefix=%rootpath%\lib\windows\vs2015"
set "projpath=%MOAI_SDK_HOME%\vs2015"


echo Creating Release Libs
msbuild %projpath%\moai.sln /verbosity:minimal /t:Moai\moai /p:Configuration=Release /p:Platform=Win32 || echo "Error during build" && exit /b 1

robocopy "%projpath%\bin\win32\Release" "%libprefix%\Release" /MIR 

echo Creating Debug Libs
msbuild %projpath%\moai.sln /verbosity:minimal /t:Moai\moai /p:Configuration=Debug /p:Platform=Win32 || echo "Error during build" && exit /b 1

robocopy "%projpath%\bin\win32\Debug" "%libprefix%\Debug" /MIR 

if not exist "%rootpath%\bin\moai.exe" copy "%projpath%\bin\win32\debug\moai.exe" "%rootpath%\bin\moai.exe"

echo "Build complete"
exit /b 0