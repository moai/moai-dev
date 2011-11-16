::================================================================::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::================================================================::

	@echo off
	setlocal
	set CYGWIN=nodosfilewarning
	call bash run.sh

	if %ERRORLEVEL% == 2 (
		set install_cmd=ant release install
		set ERRORLEVEL=0
	) else (
		set install_cmd=ant debug install	
	)

	if %ERRORLEVEL% == 0 (
		pushd build
			call ant uninstall 
			call ant clean
			call %install_cmd%
			call adb shell am start -a android.intent.action.MAIN -n @SETTING_PACKAGE@/@SETTING_PACKAGE@.MoaiActivity
			call adb logcat -c
			call adb logcat MoaiLog:V AndroidRuntime:E *:S
		popd
	)

	endlocal
	@pause
