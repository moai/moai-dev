set SDK_PATH=..\sdk

rd /s /q %SDK_PATH%\3rdparty
rd /s /q %SDK_PATH%\bin
rd /s /q %SDK_PATH%\docs
rd /s /q %SDK_PATH%\samples\hello-moai
rd /s /q %SDK_PATH%\samples\lua
rd /s /q %SDK_PATH%\samples\src
rd /s /q %SDK_PATH%\samples\config
rd /s /q %SDK_PATH%\include

del moai-sdk-windows.exe
del %SDK_PATH%\version.txt