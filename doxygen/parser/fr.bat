pushd "%~1"
if "%~5" == "false" (
	for %%i in (%~2) do perl -p -i.bak -e "BEGIN {undef $/;} s/%~3/%~4/sgmi;" "%%i"
) else (
	for /R %%i in (%~2) do perl -p -i.bak -e "BEGIN {undef $/;} s/%~3/%~4/sgmi;" "%%i"
)
del /s /q /f *.bak
popd