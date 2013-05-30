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