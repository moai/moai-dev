set -e # exit on error

rm -rf libmoai/obj
rm -rf libmoai/libs

../pito/pito ndk-libmoai
