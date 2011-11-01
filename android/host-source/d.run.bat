::================================================================::
:: Copyright (c) 2010-2011 Zipline Games, Inc.
:: All Rights Reserved.
:: http://getmoai.com
::================================================================::

	bash run.sh

	pushd build
		ant uninstall 
		ant clean
		ant debug install
		adb shell am start -a android.intent.action.MAIN -n @SETTING_PACKAGE@/@SETTING_PACKAGE@.MoaiActivity
		:: adb logcat MoaiJNI:V MoaiLog:V *:S
		adb logcat MoaiLog:V *:S
	popd

	@if %ERRORLEVEL% NEQ 0 echo There was an error. Check "README.txt" for information on settings up your environment.
	@pause

