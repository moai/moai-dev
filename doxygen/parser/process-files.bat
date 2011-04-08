pushd src-copy
for /R %%i in (*.cpp) do call ..\parser\parse-file "%%i" %1 %2
popd