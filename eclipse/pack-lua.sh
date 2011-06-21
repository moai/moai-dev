cd ..

rm -rf host-android/lua-prep

find "lua" ! -name "/.svn/" | cpio -admvp "host-android/lua-prep"

cd host-android/lua-prep/lua
rm -f ../../android-project/res/raw/lua
zip -9 -r ../../android-project/res/raw/lua . *.*

cd ../../../host-android/android-project/res/raw
mv lua.zip lua
