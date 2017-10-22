@echo off
setlocal enableextensions
call "%~dp0%\env-win.bat"

echo "Setting CMAKE bin path..."
set "PATH=%PATH%;%CMAKE_PATH%"

echo "Setting Emscripten path..."
pushd .
cd "%EMSDK_PATH%"
call emsdk_env.bat
popd                         

echo "Setting MingW Gcc path"
set "PATH=%MINGW_PATH%;%PATH%"

rem ----- Requirements Check -----

if "%MOAI_SDK_HOME%"=="" echo "Could not determine location of MOAI SDK, please set MOAI_SDK_HOME" && goto ERROR

if "%EMSCRIPTEN%"=="" echo "EMSCRIPTEN is not defined. Please set to the location of your emscripten install (path)" && goto ERROR

where mingw32-make || echo "mingw32-make is required. Install TCC Mingw from http://tdm-gcc.tdragon.net/ and add to the path" && goto ERROR

where cmake || echo "Cmake 3.2+ is required, download from cmake.org" && goto ERROR

rem ----- Build libmoai ----

pushd "%~dp0%"

set "CMAKEROOT=%cd%"

mkdir build
cd build
mkdir build-html
cd build-html

cmake ^
-DEMSCRIPTEN_ROOT_PATH=%EMSCRIPTEN% ^
-DCMAKE_TOOLCHAIN_FILE=%EMSCRIPTEN%\cmake\Modules\Platform\Emscripten.cmake ^
-DMOAI_SDK_HOME="%MOAI_SDK_HOME%" ^
-DBUILD_HTML=TRUE ^
-DCMAKE_BUILD_TYPE=Release ^
-DMOAI_CHIPMUNK=FALSE ^
-DMOAI_CURL=FALSE ^
-DMOAI_CRYPTO=FALSE ^
-DMOAI_MONGOOSE=FALSE ^
-DMOAI_OGG=FALSE ^
-DMOAI_SQLITE3=FALSE ^
-DMOAI_SFMT=FALSE ^
-DMOAI_VORBIS=FALSE ^
-DMOAI_WEBP=FALSE ^
-DMOAI_HTTP_CLIENT=FALSE ^
-DMOAI_LUAJIT=FALSE ^
-DCMAKE_INSTALL_PREFIX="%CMAKEROOT%\lib\html" ^
-G "MinGW Makefiles" ^
%CMAKEROOT%\cmake\hosts\host-html || goto ERROR

cmake --build  . --target moaijs || goto ERROR

rem Install into lib
mkdir "%CMAKEROOT%\lib\html"
copy /y moaijs.js "%CMAKEROOT%\lib\html\moaijs.js"
copy /y moaijs.wasm "%CMAKEROOT%\lib\html\moaijs.wasm"

goto END

:ERROR
exit /b 1

:END
exit /b 0
