call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
cd vs2010
msbuild moai-open.sln /p:Configuration=Release /verbosity:quiet
