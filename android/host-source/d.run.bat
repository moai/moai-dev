::================================================================::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::================================================================::

	@echo off
	setlocal
	set CYGWIN=nodosfilewarning
	call bash run.sh

	if %ERRORLEVEL% == 0 (
		pushd build
			call ant uninstall 
			call ant clean
			call ant debug install
			call adb shell am start -a android.intent.action.MAIN -n com.distinctdev.tmt2/com.distinctdev.tmt2.MoaiActivity
			:: call adb logcat MoaiJNI:V MoaiLog:V *:S
			call adb logcat MoaiLog:V *:S
		popd
	)

	endlocal
	pause