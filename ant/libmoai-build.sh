set -e # exit on error

pushd libmoai/jni > /dev/null
ndk-build -j4
popd > /dev/null
