call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
cd vs2010

msbuild moai.sln /property:Config=Debug-Lua-5.1
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Config=Release-Lua-5.1
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Config=Debug-Lua-5.2
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Config=Release-Lua-5.2
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Config=Debug-LuaJIT
if ERRORLEVEL 1 exit

msbuild moai.sln /property:Config=Release-LuaJIT
if ERRORLEVEL 1 exit