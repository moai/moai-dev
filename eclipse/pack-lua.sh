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

# adapt your project to use the below command, and it will filter out svn directories
#find "$LUA_FOLDER" ! -name "/.svn/" | cpio -admvp "eclipse/lua-prep"

cd eclipse/lua-prep
rm -f ../android-project/res/raw/lua
zip -9 -r ../android-project/res/raw/lua . *.*

cd ../../eclipse/android-project/res/raw
mv lua.zip lua

#rm -rf eclipse/lua-prep
