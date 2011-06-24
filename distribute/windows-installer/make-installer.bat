call clean-installer.bat

::read version from text file
pushd ..\..
set /a count=0

setlocal ENABLEDELAYEDEXPANSION
for /F "skip=1 tokens=2*" %%i in (version.txt) do (

	set /a count=count+1

	if "!count!" == "1" (
		set version=%%i
		if not "%%j" == "" set tag=%%j
	)
	
	if "!count!" == "2" (
		set revision=%%i
	)
	
	if "!count!" == "3" goto forDone
)
:forDone
popd

set revTagPre="(revision"
set revTagSuf="%revision%)"

if "%tag%" == "" (
	set displayName=Moai SDK %version% %revTagPre% %revTagSuf%
) else (
	set displayName=Moai SDK %version% %tag% %revTagPre% %revTagSuf%
)

:: create version file
pushd ..\sdk
del /q version.txt
echo Moai SDK>> version.txt

if not "%tag%" == "" (
	echo Version !version! !tag!>> version.txt
) else (
	echo Version !version!>> version.txt
)

echo Revision !revision!>> version.txt
popd

::push version number into installer text
copy moai.nsi moai-temp.nsi

perl -p -i.bak -e "s/\@\@DISPLAY_NAME\@\@/%displayName%/sgi;" "moai-temp.nsi"
del /q moai-temp.nsi.bak

::run NSIS script to create installer
makensis.exe "moai-temp.nsi"
del /q moai-temp.nsi

endlocal
cd distribute\windows-installer