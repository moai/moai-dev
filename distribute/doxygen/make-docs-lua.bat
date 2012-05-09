::kill existing copy of source files folder, if any
if exist src-copy rmdir /s /q src-copy

::create copy of source files
xcopy /v /i /s /y "../../src" "src-copy"

::modify copy of source files
call parser\process-files header-files.lua h
call parser\process-files source-files-cpp.lua cpp
call parser\process-files source-files-mm.lua mm

::kill existing doxygen output folder, if any
rmdir /s /q html-lua

::run doxygen on copy of source files
"C:\Program Files (x86)\doxygen\bin\doxygen.exe" doxyfile-lua

::properly intro-page escape file
copy /y intro-page.txt intro-page-temp.txt
call parser\fr "." "intro-page-temp.txt" "([\<\>\"\"\&\/\\\;\.\'\'\:\@])" "\\$1"

::load contents of intro page text file
setlocal ENABLEDELAYEDEXPANSION
set input=
for /F "tokens=*" %%i in (intro-page-temp.txt) do (
	set input=!input!\n%%i
)
del /q intro-page-temp.txt

call parser\fr "html-lua" "index.html" "(\<div class=\"contents\"\>).*?(\<\/div\>)" "$1%input%$2"

::update doxygen's version text
pushd ..\..
set /a count=0

setlocal ENABLEDELAYEDEXPANSION
for /F "skip=1 tokens=2*" %%i in (version.txt) do (

	set /a count=count+1

	if "!count!" == "1" (
		set version=%%i
		if not "%%j" == "" set tag=%%j
	)
	
	if "!count!" == "2" (
		set revision=%%i
	)
	
	if "!count!" == "3" goto forDone
)
:forDone
popd

set revTagPre=\(revision
set revTagSuf=%revision%\)

if "%tag%" == "" (
	set versionText=%version% %revTagPre% %revTagSuf%
) else (
	set versionText=%version% %tag% %revTagPre% %revTagSuf%
)

call parser\fr "html-lua" "*.html" "\@\@VERSION\@\@" "%versionText%"

::clean up doxygen docs
call parser\fr "html-lua" "*.html" "Static .*? Member Functions" "Function List"
call parser\fr "html-lua" "*.html" "Member Function Documentation" "Function Documentation"
call parser\fr "html-lua" "*.html" "SUPPRESS_EMPTY_FILE_WARNING" ""
call parser\fr "html-lua" "*.html" "static int(.*?\>)_(.*?\<\/a\>)" "$1$2"
call parser\fr "html-lua" "*.html" "\(lua_State \*L\)" ""
call parser\fr "html-lua" "*.html" "int MOAI.*?::_(.*?\<\/td\>)" "$1"
call parser\fr "html-lua" "*.html" "(\>)lua_State \*.*?(\<)" "$1$2"
call parser\fr "html-lua" "*.html" "\<em\>L\<\/em\>" ""
call parser\fr "html-lua" "*.html" "\<td class=\"paramtype\"\>\<\/td\>" ""
call parser\fr "html-lua" "*.html" "\<td class=\"paramname\"\>\<\/td\>" ""
call parser\fr "html-lua" "*.html" "\<td\>\(\<\/td\>" ""
call parser\fr "html-lua" "*.html" "\<td\>\)\<\/td\>" ""
call parser\fr "html-lua" "*.html" "\[static, private]" ""
call parser\fr "html-lua" "*.html" "\[static, protected]" ""
call parser\fr "html-lua" "*.html" "\[private, static]" ""
call parser\fr "html-lua" "*.html" "\[protected, static]" ""
call parser\fr "html-lua" "*.html" "(\>)_(.*?\<\/a\>)" "$1$2"

endlocal