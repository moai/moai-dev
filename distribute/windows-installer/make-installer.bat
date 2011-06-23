set SDK_PATH=..\sdk

::clean away the existing installer (if any)
call clean-installer

::copy all needed files into the installer folder
xcopy /cery ..\..\3rdparty\glut-3.7.6-bin %SDK_PATH%\3rdparty\glut-3.7.6\

xcopy /cery ..\..\vs2008\bin\Win32\release\lua.exe %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\luac.exe %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\moai-dll.dll %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\moai-dll.lib %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\moai.exe %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\lua-5.1.3.dll %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\lua-5.1.3.dll.lib %SDK_PATH%\bin\
xcopy /cery ..\..\vs2008\bin\Win32\release\glut32.dll %SDK_PATH%\bin\

xcopy /cery ..\doxygen\html-lua\html %SDK_PATH%\docs\html\
xcopy /cery ..\..\samples\basics %SDK_PATH%\samples\lua\
xcopy /cery ..\..\samples\hello-moai %SDK_PATH%\samples\lua\hello-moai\
xcopy /cery ..\..\samples\config %SDK_PATH%\samples\lua\config\
xcopy /cery ..\..\src\aku\AKUGlut.* %SDK_PATH%\samples\hosts\src\aku\

xcopy /cery ..\..\src\aku\AKU.h %SDK_PATH%\include\aku\
 
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
pushd %SDK_PATH%
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