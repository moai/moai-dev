echo "Setting up MoaiUtil path..."

UTIL_PATH=$(dirname "${BASH_SOURCE[0]}")
UTIL_PATH=$(cd $UTIL_PATH/../util; pwd)
export PATH=$PATH:$UTIL_PATH

echo "getting latest cmake"
pushd ~
if [ ! -d "cmake-3.1.3-Linux-x86_64/bin"]; then
wget http://www.cmake.org/files/v3.1/cmake-3.1.3-Linux-x86_64.tar.gz
tar xvf cmake-3.1.3-Linux-x86_64.tar.gz
fi
cd cmake-3.1.3-Linux-x86_64/bin
export PATH=$(pwd):$PATH
popd

if [ -z "$ANDROID_NDK" ]; then
echo "getting ndk"
pushd ~
wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2 -O ndk.tgz
tar -xf ndk.tgz
export ANDROID_NDK=`pwd`/android-ndk-r9c
popd
fi

pushd `dirname $0`
bash build-android.sh
echo Android Build Successful
popd

echo Creating and building android host
sudo chmod a+x util/moai
sudo chmod a+x util/moaiutil

export MOAI_ROOT=$(pwd)
pushd ~
mkdir testhost
cd testhost
cp -R $MOAI_ROOT/samples/hello-moai src/
moaiutil host init
moaiutil host build android-gradle
popd