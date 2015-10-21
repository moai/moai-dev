set -e # exit on error

rm -rf libmoai/obj
rm -rf libmoai/libs

../util/pito ant-libmoai
