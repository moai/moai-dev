pushd %~dp0
del /q input.%3
del /q output.%3
copy /y %1 input.%3 
bin\moai %2
copy /y output.%3 %1
del /q input.%3
del /q output.%3
popd