setlocal
set PATH=%ANDROID_NDK%;%PATH%
pushd libmoai\jni
ndk-build -j4
popd

