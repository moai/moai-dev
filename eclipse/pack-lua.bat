::----------------------------------------------------------------::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::----------------------------------------------------------------::

@echo off
set LUA_FOLDER=device-info

::----------------------------------------------------------------::
pushd ..

	if exist eclipse\lua-prep (
		del /Q /S eclipse\lua-prep\*.*
		rd /Q /S eclipse\lua-prep
	)

	echo ----------------------------------------------------------------
	echo Copying Resources
	echo ----------------------------------------------------------------
	xcopy "%LUA_FOLDER%" "eclipse\lua-prep" /E /C /I /K /Y /EXCLUDE:eclipse\excludes.txt

	echo ----------------------------------------------------------------
	echo Packaging Resources
	echo ----------------------------------------------------------------
	
	pushd eclipse\lua-prep
		if exist ..\android-project\res\raw\lua del /Q ..\android-project\res\raw\lua
		zip -9 -r ../android-project/res/raw/lua . *.*
	popd

	pushd eclipse\android-project\res\raw
		ren lua.zip lua
	popd
	
	echo ----------------------------------------------------------------
	echo Cleaning Up
	echo ----------------------------------------------------------------
	
	if exist eclipse\lua-prep (
		del /Q /S eclipse\lua-prep\*.*
		rd /Q /S eclipse\lua-prep
	)
popd