call clean_installer

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
xcopy /cery ..\src\aku\AKUGlut.* release\samples\src\aku\

xcopy /cery ..\src\aku\AKU.h release\include\aku\

makensis.exe "moai.nsi"
