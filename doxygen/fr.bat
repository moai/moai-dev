CD html-lua
FOR /R %%i IN (*.html) DO perl -p -i.bak -e "s/%~1/%~2/ms;" %%i
CD ..