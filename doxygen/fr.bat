pushd %~1
for /R %%i in (%~2) do perl -p -i.bak -e "BEGIN {undef $/;} s/%~3/%~4/sgmi;" %%i
del /s /q /f *.bak
popd