::clean away the existing installer (if any)
call clean_installer

::copy all needed files into the installer folder
xcopy /cery ..\3rdparty\glut-3.7.6-bin release\3rdparty\glut-3.7.6\

xcopy /cery ..\vs2008\bin\Win32\release\lua.exe release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\luac.exe release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\moai-dll.dll release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\moai-dll.lib release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\moai.exe release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\lua-5.1.3.dll release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\lua-5.1.3.dll.lib release\bin\
xcopy /cery ..\vs2008\bin\Win32\release\glut32.dll release\bin\

xcopy /cery ..\doxygen\html-lua\html release\docs\html\
xcopy /cery ..\samples\basics release\samples\lua\
xcopy /cery ..\samples\hello-moai release\samples\hello-moai\
xcopy /cery ..\samples\config release\samples\config\
xcopy /cery ..\src\aku\AKUGlut.* release\samples\src\aku\

xcopy /cery ..\src\aku\AKU.h release\include\aku\

::read version from text file
pushd ..
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
	REM set versionPath=v%version%.%revision%
) else (
	set displayName=Moai SDK %version% %tag% %revTagPre% %revTagSuf%
	REM set versionPath=v%version%.%revision% %tag%
)

:: create version file
pushd release
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

REM perl -p -i.bak -e "s/\@\@VERSION_PATH\@\@/%versionPath%/sgi;" "moai-temp.nsi"
REM del /q moai-temp.nsi.bak

::run NSIS script to create installer
makensis.exe "moai-temp.nsi"
del /q moai-temp.nsi

endlocal