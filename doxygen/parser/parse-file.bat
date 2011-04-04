pushd %~dp0
del /q input.cpp
copy /y %1 input.cpp 
bin\moai main.lua
copy /y input.cpp %1
del /q input.cpp
popd