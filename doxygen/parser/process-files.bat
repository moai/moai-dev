pushd src-copy
for /R %%i in (*.%2) do call ..\parser\parse-file "%%i" %1 %2
popd