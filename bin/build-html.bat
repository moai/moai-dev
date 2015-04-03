@echo off
setlocal enableextensions

rem Prerequisites

if "%EMSCRIPTEN%"=="" echo "EMSCRIPTEN is not defined. Please set to the location of your emscripten install (path)" && goto ERROR

where mingw32-make || echo "mingw32-make is required. Install TCC Mingw from http://tdm-gcc.tdragon.net/ and add to the path" && goto ERROR

where cmake || echo "Cmake 2.8.11+ is required, download from cmake.org" && goto ERROR


:COMPILE

cd %~dp0%/..

set MOAIROOT=%cd%

mkdir build
cd build
mkdir build-html
cd build-html
cmake ^
-DEMSCRIPTEN_ROOT_PATH=%EMSCRIPTEN% ^
-DCMAKE_TOOLCHAIN_FILE=%EMSCRIPTEN%\cmake\Modules\Platform\Emscripten.cmake ^
-DBUILD_HTML=TRUE ^
-DCMAKE_BUILD_TYPE=Release ^
-DMOAI_CHIPMUNK=FALSE ^
-DMOAI_CURL=FALSE ^
-DMOAI_CRYPTO=FALSE ^
-DMOAI_LIBCRYPTO=FALSE ^
-DMOAI_EXPAT=FALSE ^
-DMOAI_MONGOOSE=FALSE ^
-DMOAI_OGG=FALSE ^
-DMOAI_OPENSSL=FALSE ^
-DMOAI_SQLITE3=FALSE ^
-DMOAI_SFMT=FALSE ^
-DMOAI_VORBIS=FALSE ^
-DMOAI_WEBP=FALSE ^
-DMOAI_HTTP_CLIENT=FALSE ^
-DMOAI_LUAJIT=FALSE ^
-DCMAKE_INSTALL_PREFIX="%MOAIROOT%\lib\html" ^
-G "MinGW Makefiles" ^
%MOAIROOT%\cmake\hosts\host-html || goto ERROR


cmake --build  . --target moaijs || goto ERROR


rem Install into lib

mkdir %MOAIROOT%\lib\html
copy /y moaijs.js %MOAIROOT%\lib\html\moaijs.js 

goto END

:ERROR
exit /b 1

:END

