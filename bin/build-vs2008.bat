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

rmdir release /s

mkdir release
mkdir release/vs2008
mkdir release/vs2008/bin/
mkdir release/vs2008/bin/Lua-5.1/
mkdir release/vs2008/bin/Lua-5.2/
mkdir release/vs2008/bin/Lua-JIT/

cp vs2008\bin\Win32\Release-Lua-5.1\moai.exe release/vs2008/bin/Lua-5.1/
cp vs2008\bin\Win32\Release-Lua-5.1\glut32.dll release/vs2008/bin/Lua-5.1/
cp vs2008\bin\Win32\Release-Lua-5.2\moai.exe release/vs2008/bin/Lua-5.2/
cp vs2008\bin\Win32\Release-Lua-5.2\glut32.dll release/vs2008/bin/Lua-5.2/
cp vs2008\bin\Win32\Release-LuaJIT\moai.exe release/vs2008/bin/Lua-JIT/
cp vs2008\bin\Win32\Release-LuaJIT\glut32.dll release/vs2008/bin/Lua-JIT/

mkdir release/vs2008/lib/
mkdir release/vs2008/lib/Lua-5.1/
mkdir release/vs2008/lib/Lua-5.2/
mkdir release/vs2008/lib/Lua-JIT/
cp vs2008\bin\Win32\Release-Lua-5.1\*.lib release/vs2008/lib/Lua-5.1/
cp vs2008\bin\Win32\Release-Lua-5.2\*.lib release/vs2008/lib/Lua-5.2/
cp vs2008\bin\Win32\Release-LuaJIT\*.lib release/vs2008/lib/Lua-JIT/
