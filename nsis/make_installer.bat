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
xcopy /cery ..\src\aku\AKUGlut.* release\samples\src\aku\

xcopy /cery ..\src\aku\AKU.h release\include\aku\

::push version number into installer text
::read version from text file
pushd ..
for /F "skip=1 tokens=2*" %%i in (version.txt) do (
	if "%%j" == "" (
		set displayName=Moai SDK %%i
		set versionPath=v%%i
	) else (
		set displayName=Moai SDK %%i %%j
		set versionPath=v%%i-%%j
	)
	goto forDone
)
:forDone
popd

copy moai.nsi moai-temp.nsi

perl -p -i.bak -e "s/\@\@DISPLAY_NAME\@\@/%displayName%/sgi;" "moai-temp.nsi"
del /q moai-temp.nsi.bak

perl -p -i.bak -e "s/\@\@VERSION_PATH\@\@/%versionPath%/sgi;" "moai-temp.nsi"
del /q moai-temp.nsi.bak

::run NSIS script to create installer
makensis.exe "moai-temp.nsi"
del /q moai-temp.nsi
