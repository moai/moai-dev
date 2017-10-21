
PITO_ROOT=$(cd `dirname $0`/.. && pwd)
: "${MOAI_SDK_HOME:?Please set MOAI_SDK_HOME variable to point to your Moai SDK }"
: "${ANDROID_NDK:?You need to set ANDROID_NDK to point to your ndk path. Try using env-local.sh }"

#----- Build libmoai using sdk methods -----

export PATH=$ANDROID_NDK:$PATH
echo $PATH
echo "Builing libmoai"

pushd $MOAI_SDK_HOME/ant
if [ "$1" == "--clean" ]; then
  ./libmoai-clean.sh
fi

if [ ! -e "$MOAI_SDK_HOME/ant/libmoai/jni/libraries.mk" ]; then
   ./libmoai-clean.sh
fi

./libmoai-build.sh || ( echo "Android NDK build failed" && exit 1)

mkdir -p $PITO_ROOT/lib/android/libs
mkdir -p $PITO_ROOT/lib/android/obj

cp -a libmoai/libs/ $PITO_ROOT/lib/android/libs
(cd libmoai/obj && find . -type f -name \*.a -exec tar cf - {} +) | (cd $PITO_ROOT/lib/android/obj && tar xf -)

popd
