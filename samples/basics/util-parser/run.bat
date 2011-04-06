::----------------------------------------------------------------::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::----------------------------------------------------------------::

@echo off

:: verify paths
if not exist %MOAI_BIN%\moai.exe (
	echo.
	echo --------------------------------------------------------------------------------
	echo ERROR: The MOAI_BIN environment variable is pointing to an invalid path.
	echo Please point it at a folder containing moai.exe.
	echo --------------------------------------------------------------------------------
	echo.
	goto end
)

if not exist %MOAI_CONFIG% (
	echo.
	echo -------------------------------------------------------------------------------
	echo WARNING: The MOAI_CONFIG environment variable is pointing to an invalid path.
	echo You should point it at a folder containing config.lua.
	echo -------------------------------------------------------------------------------
	echo.
)

:: run moai
%MOAI_BIN%\moai "%MOAI_CONFIG%\config.lua" "main.lua"

:end
pause