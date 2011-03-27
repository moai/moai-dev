call fr "Static .* Member Functions" "Function List"
call fr "Member Function Documentation" "Function Documentation"
call fr "SUPPRESS_EMPTY_FILE_WARNING" ""
call fr "static int(.*\>)_(.*\<\/a\>)" "$1$2"
call fr "\(lua_State \*L\)" "\(\)"
call fr "int MOAI.*::_(.*\<\/td\>)" "$1"
call fr "(\>)lua_State \*.*(\<)" "$1$2"
call fr "\<em\>L\<\/em\>" ""
call fr "\<td class=\"paramtype\"\>\<\/td\>" ""
call fr "\<td class=\"paramname\"\>\<\/td\>" ""
call fr "\<td\>\(\<\/td\>" ""
call fr "\<td\>\)\<\/td\>" ""
call fr "(\<code\>).*\[.*static.*\].*(\<\/code\>)" "$1$2"
call fr "(\>)_(.*\(\))" "$1$2"

::cleanup
del /s /q /f *.bak
