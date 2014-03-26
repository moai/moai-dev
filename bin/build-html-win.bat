@echo off
if NOT "%EMSCRIPTEN_HOME%"=="" goto COMPILE
echo "EMSCRIPTEN_HOME is not defined. Please set to the location of your emscripten install (path)"
goto :END

where mingw32-make
if %ERRORLEVEL%==0 GOTO COMPILE
echo "mingw32-make is required. Install TCC Mingw"
goto ERROR

:COMPILE

cd %~dp0/..
cd cmake
rmdir /s /q build
mkdir build
cd build
cmake ^
-DEMSCRIPTEN_ROOT_PATH=%EMSCRIPTEN_HOME% ^
-DCMAKE_TOOLCHAIN_FILE=%EMSCRIPTEN_HOME%\cmake\Platform\Emscripten.cmake ^
-DBUILD_HTML=TRUE ^
-DMOAI_BOX2D=TRUE ^
-DMOAI_CHIPMUNK=FALSE ^
-DMOAI_CURL=FALSE ^
-DMOAI_CRYPTO=FALSE ^
-DMOAI_EXPAT=FALSE ^
-DMOAI_FREETYPE=TRUE ^
-DMOAI_JSON=TRUE ^
-DMOAI_JPG=FALSE ^
-DMOAI_MONGOOSE=FALSE ^
-DMOAI_OGG=FALSE ^
-DMOAI_OPENSSL=FALSE ^
-DMOAI_SQLITE3=FALSE ^
-DMOAI_TINYXML=TRUE ^
-DMOAI_PNG=TRUE ^
-DMOAI_SFMT=FALSE ^
-DMOAI_VORBIS=FALSE ^
-DMOAI_HTTP_CLIENT=FALSE ^
-DMOAI_UNTZ=TRUE ^
-G "MinGW Makefiles" ^
..\

IF %errorlevel% NEQ 0 GOTO ERROR

cmake --build  . --target host-html-template

IF %errorlevel% NEQ 0 GOTO ERROR

goto END

:ERROR
exit /b 1

:END

