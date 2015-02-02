@echo off
set SCRIPT_DIR=%~dp0%
set INVOKE_DIR=%CD%
set SDK_HOME=%SCRIPT_DIR%\..\
set MOAI_CMD=%1


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
