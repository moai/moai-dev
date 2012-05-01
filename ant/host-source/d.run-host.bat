::================================================================::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::================================================================::

	@echo off
	setlocal
	set CYGWIN=nodosfilewarning
	bash run-host.sh %*

	if %ERRORLEVEL% == 2 (
		set install_cmd=ant release install
		set ERRORLEVEL=0
	) else (
		set install_cmd=ant debug install	
	)

	if %ERRORLEVEL% == 0 (
		pushd build\project
			call ant uninstall 
			call ant clean
			call %install_cmd%
			call adb shell am start -a android.intent.action.MAIN -n com.distinctdev.tmt2/com.distinctdev.tmt2.MoaiActivity
			call adb logcat -c
			call adb logcat MoaiLog:V AndroidRuntime:E *:S
		popd
	)

	endlocal
	@pause
