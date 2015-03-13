::----------------------------------------------------------------::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::----------------------------------------------------------------::

@echo off

:: verify paths
if not exist "%MOAI_BIN%\moai-test.exe" (
	echo.
	echo --------------------------------------------------------------------------------
	echo ERROR: The MOAI_BIN environment variable either doesn't exist or it's pointing
	echo to an invalid path. Please point it at a folder containing moai-test.exe.
	echo --------------------------------------------------------------------------------
	echo.
	goto end
)

:: run moai
"%MOAI_BIN%\moai-test" "main-staging.lua"

:end
pause