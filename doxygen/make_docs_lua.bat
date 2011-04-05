::kill existing copy of source files folder, if any
if exist src-copy rmdir /s /q src-copy

::create copy of source files
xcopy /v /i /s /y "../src" "src-copy"

::modify copy of source files
call parser\process-files

::kill existing doxygen output folder, if any
rmdir /s /q html-lua

::run doxygen on copy of source files
doxygen doxyfile-lua

::modify doxygen's output
REM call parser\fr "html-lua" "*.html" "Static .*? Member Functions" "Function List"
REM pause
REM call parser\fr "html-lua" "*.html" "Member Function Documentation" "Function Documentation"
REM pause
REM call parser\fr "html-lua" "*.html" "SUPPRESS_EMPTY_FILE_WARNING" ""
REM pause
REM call parser\fr "html-lua" "*.html" "static int(.*?\>)_(.*?\<\/a\>)" "$1$2"
REM pause
REM call parser\fr "html-lua" "*.html" "\(lua_State \*L\)" ""
REM pause
REM call parser\fr "html-lua" "*.html" "int MOAI.*?::_(.*?\<\/td\>)" "$1"
REM pause
REM call parser\fr "html-lua" "*.html" "(\>)lua_State \*.*?(\<)" "$1$2"
REM pause
REM call parser\fr "html-lua" "*.html" "\<em\>L\<\/em\>" ""
REM pause
REM call parser\fr "html-lua" "*.html" "\<td class=\"paramtype\"\>\<\/td\>" ""
REM pause
REM call parser\fr "html-lua" "*.html" "\<td class=\"paramname\"\>\<\/td\>" ""
REM pause
REM call parser\fr "html-lua" "*.html" "\<td\>\(\<\/td\>" ""
REM pause
REM call parser\fr "html-lua" "*.html" "\<td\>\)\<\/td\>" ""
REM pause
REM call parser\fr "html-lua" "*.html" "\[.*?static.*?\]" ""
REM pause
REM call parser\fr "html-lua" "*.html" "(\>)_(.*?\(\))" "$1$2"
REM pause
