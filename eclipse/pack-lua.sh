#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

LUA_FOLDER=samples/basics/anim-basic

#----------------------------------------------------------------#
cd ..

rm -rf eclipse/lua-prep

cp -r "$LUA_FOLDER" "eclipse/lua-prep"
rm -f eclipse/lua-prep/.DS_Store

# adapt your project to use the below command, and it will filter out svn directories
#find "$LUA_FOLDER" ! -name "/.svn/" | cpio -admvp "eclipse/lua-prep"

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