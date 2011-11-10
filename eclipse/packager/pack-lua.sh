#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

cd ../android-project
read LUA_FOLDER < moai-target

#----------------------------------------------------------------#
cd ../..

rm -rf eclipse/lua-prep

cp -r "$LUA_FOLDER" "eclipse/lua-prep"
rm -f eclipse/lua-prep/.DS_Store
rm -f eclipse/lua-prep/run.bat
rm -f eclipse/lua-prep/run.sh

echo ----------------------------------------------------------------
echo Copying Android Config File
echo ----------------------------------------------------------------
cp "samples/config/config-android.lua" "eclipse/lua-prep/config-android.lua"
echo config-android.lua

echo ----------------------------------------------------------------
echo Packaging Resources
echo ----------------------------------------------------------------

cd eclipse/lua-prep
rm -f ../android-project/res/raw/lua
zip -9 -r ../android-project/res/raw/lua . *.*

cd ..
rm -rf lua-prep

cd ../eclipse/android-project/res/raw
mv lua.zip lua