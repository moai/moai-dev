call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
cd vs2008

msbuild moai.sln /property:Configuration=Debug-Lua-5.1
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Configuration=Release-Lua-5.1
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Configuration=Debug-Lua-5.2
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Configuration=Release-Lua-5.2
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Configuration=Debug-LuaJIT
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Configuration=Release-LuaJIT
if ERRORLEVEL 1 exit

cd ..

IF EXIST release rmdir release /s /q

mkdir release
cd release

mkdir vs2008
cd vs2008
mkdir bin
cd bin
mkdir Lua-5.1
mkdir Lua-5.2
mkdir Lua-JIT
cd ..
mkdir lib
cd lib
mkdir Lua-5.1
mkdir Lua-5.2
mkdir Lua-JIT

cd ..
cd ..
cd ..

xcopy "vs2008\bin\Win32\Release-Lua-5.1\moai.exe" "release/vs2008/bin/Lua-5.1"
xcopy "vs2008\bin\Win32\Release-Lua-5.1\glut32.dll" "release/vs2008/bin/Lua-5.1"
xcopy "vs2008\bin\Win32\Release-Lua-5.2\moai.exe" "release/vs2008/bin/Lua-5.2"
xcopy "vs2008\bin\Win32\Release-Lua-5.2\glut32.dll" "release/vs2008/bin/Lua-5.2"
xcopy "vs2008\bin\Win32\Release-LuaJIT\moai.exe" "release/vs2008/bin/Lua-JIT"
xcopy "vs2008\bin\Win32\Release-LuaJIT\glut32.dll" "release/vs2008/bin/Lua-JIT"

xcopy "vs2008\bin\Win32\Release-Lua-5.1\*.lib" "release/vs2008/lib/Lua-5.1"
xcopy "vs2008\bin\Win32\Release-Lua-5.2\*.lib" "release/vs2008/lib/Lua-5.2"
xcopy "vs2008\bin\Win32\Release-LuaJIT\*.lib" "release/vs2008/lib/Lua-JIT"
