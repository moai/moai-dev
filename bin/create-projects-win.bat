:: Remember the original working directory
pushd .

:: Move to cmake directory
cd "%~dp0%..\cmake"

:: Delete and re-create the target directory
set targetDir=projects\vs2013
if exist %targetDir% rmdir /s /q %targetDir%
md %targetDir%

:: Move to target directory, then call cmake from there
cd %targetDir%
cmake ^
-G "Visual Studio 11" ^
-DBUILD_WINDOWS=TRUE ^
-DMOAI_BOX2D=TRUE ^
-DMOAI_CHIPMUNK=FALSE ^
-DMOAI_CURL=FALSE ^
-DMOAI_CRYPTO=TRUE ^
-DMOAI_EXPAT=TRUE ^
-DMOAI_FREETYPE=TRUE ^
-DMOAI_JSON=TRUE ^
-DMOAI_JPG=TRUE ^
-DMOAI_LUAEXT=TRUE ^
-DMOAI_MONGOOSE=TRUE ^
-DMOAI_OGG=TRUE ^
-DMOAI_OPENSSL=FALSE ^
-DMOAI_SQLITE3=TRUE ^
-DMOAI_TINYXML=TRUE ^
-DMOAI_PNG=TRUE ^
-DMOAI_SFMT=TRUE ^
-DMOAI_VORBIS=TRUE ^
-DMOAI_UNTZ=TRUE ^
-DMOAI_LUAJIT=TRUE ^
..\..\

:: Restore original working directory
popd
