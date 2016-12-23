
pushd 
cd %~dp0%..\..\
setlocal

call bin\env-win.bat || goto :error
call bin\build-all-windows.bat || goto :error

popd 

:error
endlocal
echo "error during build"
exit /b 1

:end
endlocal