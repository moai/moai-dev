::----------------------------------------------------------------::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::----------------------------------------------------------------::

@echo off
if not exist run.bat (
	echo.
	echo ----------------------------------------------------------------
	echo USAGE: put the run.bat file you want to copy in the same folder
	echo as this file. Magic!
	echo ----------------------------------------------------------------
	echo.
	goto end
)

for /r %%i in (run.bat) do if exist "%%~dpirun.bat" copy /y run.bat "%%~dpirun.bat"

:end
pause