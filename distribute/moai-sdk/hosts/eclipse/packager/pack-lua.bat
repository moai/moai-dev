::----------------------------------------------------------------::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::----------------------------------------------------------------::

@echo off

pushd ..\android-project
	set	/p LUA_FOLDER= <moai-target
popd

::----------------------------------------------------------------::
set LUA_PREP_PATH=hosts\eclipse\lua-prep

pushd ..\..\..

	if exist %LUA_PREP_PATH% (
		del /Q /S %LUA_PREP_PATH%\*.*
		rd /Q /S %LUA_PREP_PATH%
	)

	echo ----------------------------------------------------------------
	echo Copying Resources
	echo ----------------------------------------------------------------
	xcopy "%LUA_FOLDER%" "%LUA_PREP_PATH%" /E /C /I /K /Y /EXCLUDE:hosts\eclipse\packager\excludes.txt

	echo ----------------------------------------------------------------
	echo Copying Android Config File
	echo ----------------------------------------------------------------
	copy "samples\config\config-android.lua" "%LUA_PREP_PATH%\config-android.lua" /Y
	
	echo ----------------------------------------------------------------
	echo Packaging Resources
	echo ----------------------------------------------------------------
	
	pushd %LUA_PREP_PATH%
		if exist ..\android-project\res\raw\lua del /Q ..\android-project\res\raw\lua
		zip -9 -r ../android-project/res/raw/lua . *.*
	popd

	pushd hosts\eclipse\android-project\res\raw
		ren lua.zip lua
	popd
	
	echo ----------------------------------------------------------------
	echo Cleaning Up
	echo ----------------------------------------------------------------
	
	if exist %LUA_PREP_PATH% (
		del /Q /S %LUA_PREP_PATH%\*.*
		rd /Q /S %LUA_PREP_PATH%
	)
popd