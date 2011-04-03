::kill existing copy of source files folder, if any
if exist src-copy rmdir /s /q src-copy

::create copy of source files
xcopy /v /i /s /y "../src" "src-copy"

::modify copy of source files
set PARAM_EXP_PREFIX=(\@name.*?\()(.*?)\s*?\).*?\@param
set PARAM_EXP_SUFFIX=\s*(\w*?)\s*?\@type\s*(\w*)(.*?)\n(.*?)\@return
set PARAM_EXP_REPLACE=$1 $2, $3 \)\n$6\@param $3 \($4\) $5\n\t\@return

call bin\fr "src-copy" "*" "(\@name\s*?(\w*?))\n" "$1 \(\)\n"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%1%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%2%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%3%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%4%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%5%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%6%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%7%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%8%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" "%PARAM_EXP_PREFIX%9%PARAM_EXP_SUFFIX%" "%PARAM_EXP_REPLACE%"
call bin\fr "src-copy" "*" " , " ", "
call bin\fr "src-copy" "*" "(\@name.*?\() ,(.*?)\)" "$1$2\)"
call bin\fr "src-copy" "*" "\@name\s*?(\w*?.*?)\)" "\@brief\n\t\<tt\>function$1\)\<\/tt\>\\n\n\t\\n"
call bin\fr "src-copy" "*" "\@text\s*(\w{1}.*?)\n" "$1\n"
call bin\fr "src-copy" "*" "(\@return.*?)(\w{1}.*?)\@type\s*?(\w*?)\n" "$1$2\($3\)\n"

::kill existing doxygen output folder, if any
rmdir /s /q html-lua

::run doxygen on copy of source files
doxygen doxyfile-lua

::modify doxygen's output
call bin/fr "html-lua" "*.html" "Static .*? Member Functions" "Function List"
call bin/fr "html-lua" "*.html" "Member Function Documentation" "Function Documentation"
call bin/fr "html-lua" "*.html" "SUPPRESS_EMPTY_FILE_WARNING" ""
call bin/fr "html-lua" "*.html" "static int(.*?\>)_(.*?\<\/a\>)" "$1$2"
call bin/fr "html-lua" "*.html" "\(lua_State \*L\)" ""
call bin/fr "html-lua" "*.html" "int MOAI.*?::_(.*?\<\/td\>)" "$1"
call bin/fr "html-lua" "*.html" "(\>)lua_State \*.*?(\<)" "$1$2"
call bin/fr "html-lua" "*.html" "\<em\>L\<\/em\>" ""
call bin/fr "html-lua" "*.html" "\<td class=\"paramtype\"\>\<\/td\>" ""
call bin/fr "html-lua" "*.html" "\<td class=\"paramname\"\>\<\/td\>" ""
call bin/fr "html-lua" "*.html" "\<td\>\(\<\/td\>" ""
call bin/fr "html-lua" "*.html" "\<td\>\)\<\/td\>" ""
call bin/fr "html-lua" "*.html" "\[.*?static.*?\]" ""
call bin/fr "html-lua" "*.html" "(\>)_(.*?\(\))" "$1$2"
