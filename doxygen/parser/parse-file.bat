pushd %~dp0
del /q input.cpp
del /q output.cpp
copy /y %1 input.cpp 
bin\moai main.lua
copy /y output.cpp %1
del /q input.cpp
del /q output.cpp
popd