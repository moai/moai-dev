cd %~dp0%..

setlocal

echo "Building windows libs"

call bin\build-windows.bat vs2013 || goto :error

echo "windows lib complete"

echo "Creating windows host"

call moaiutil.bat host-windows-vs2013 || goto :error

echo "Windows host Complete"

echo "Building android libs"

call bin\build-android.bat || goto :error

echo "Android lib complete"

echo "Creating android host"

call moaiutil.bat host-android-gradle || goto :error

echo "android host Complete"






:error
endlocal
echo "error during build"
exit /b 1

:end
endlocal