if not exist input.cpp copy input.test.cpp input.cpp
bin\moai source-files.lua
::call fr "." "output.cpp" "\r\n" "\n" "false"
pause