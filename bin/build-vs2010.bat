call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
cd vs2010

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

IF EXIST release rmdir release /s /q

mkdir release
cd release

mkdir vs2010
cd vs2010
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

xcopy "vs2010\bin\Win32\Release-Lua-5.1\moai.exe" "release/vs2010/bin/Lua-5.1"
xcopy "vs2010\bin\Win32\Release-Lua-5.1\glut32.dll" "release/vs2010/bin/Lua-5.1"
xcopy "vs2010\bin\Win32\Release-Lua-5.2\moai.exe" "release/vs2010/bin/Lua-5.2"
xcopy "vs2010\bin\Win32\Release-Lua-5.2\glut32.dll" "release/vs2010/bin/Lua-5.2"
xcopy "vs2010\bin\Win32\Release-LuaJIT\moai.exe" "release/vs2010/bin/Lua-JIT"
xcopy "vs2010\bin\Win32\Release-LuaJIT\glut32.dll" "release/vs2010/bin/Lua-JIT"

xcopy "vs2010\bin\Win32\Release-Lua-5.1\*.lib" "release/vs2010/lib/Lua-5.1"
xcopy "vs2010\bin\Win32\Release-Lua-5.2\*.lib" "release/vs2010/lib/Lua-5.2"
xcopy "vs2010\bin\Win32\Release-LuaJIT\*.lib" "release/vs2010/lib/Lua-JIT"
