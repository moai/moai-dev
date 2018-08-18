


uname -a | grep -i Darwin && ENVSCRIPT=./env-osx.sh || ENVSCRIPT=./env-linux.sh
source $ENVSCRIPT

# requirements
: "${MOAI_SDK_HOME:?Please set MOAI_SDK_HOME variable to point to your Moai SDK }"
: "${ANDROID_NDK:?You need to set ANDROID_NDK to point to your ndk path. Try using env-local.sh }"

javac -version || { echo "Can't find a JDK" ; exit 1; }
cmake --version || { echo "Can't find cmake"; exit 1; }

#----- Build libmoai using sdk methods -----
pushd `dirname $0`

CMAKEROOT=$(pwd)

mkdir -p build/build-android
cd build/build-android

BUILDROOT=$(pwd)

# for a in x86 armeabi-v7a arm64-v8a; do
for a in x86;  do
echo "building for :$a:"
   cmake \
    -H$CMAKEROOT/cmake/hosts/host-android \
    -B$BUILDROOT \
    -G"Unix Makefiles" \
    -DANDROID_ABI=$a \
    -DANDROID_NDK=$ANDROID_NDK \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$CMAKEROOT/lib/android/$a" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_PLATFORM=android-17 \
    -DBUILD_ANDROID=TRUE \
    -DMOAI_ANDROID=TRUE \
    -DANDROID_STL=c++_static \
    -DMOAI_SDK_HOME=$MOAI_SDK_HOME \
    -DHOST_ROOT=$MOAI_SDK_HOME/src || { echo "error running cmake"; exit 1; }

   make moai -j3 || { echo "error building libmoai"; exit 1; }
done

popd
