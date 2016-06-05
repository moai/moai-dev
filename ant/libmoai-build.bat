setlocal
set PATH=%ANDROID_NDK%;%PATH%
pushd libmoai\jni
ndk-build -j4
if ERRORLEVEL 1 (
   popd
   exit /b 1
)
popd

