@echo off
setlocal 
set SCRIPT_DIR=%~dp0%
set INVOKE_DIR=%CD%
set SDK_HOME=%SCRIPT_DIR%\..\
set MOAI_CMD=%1

rem Moaiutil scripts assume moaiutil is on the path
set PATH=%PATH%;%SCRIPT_DIR%

set args=%INVOKE_DIR% %SDK_HOME% %MOAI_CMD%

shift

:parse
if "%~1" neq "" (
   set args=%args% %1
   shift 
   goto :parse
)
pushd %SCRIPT_DIR% 
moai moaiutil.lua %args%
popd 

endlocal