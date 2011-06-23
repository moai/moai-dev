#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

./clean-sdk.sh

cp -R ../../3rdparty/glut-3.7.6-bin sdk/3rdparty/glut-3.7.6/

cp -R ../../vs2008/bin/Win32/release/lua.exe sdk/bin/
cp -R ../../vs2008/bin/Win32/release/luac.exe sdk/bin/
cp -R ../../vs2008/bin/Win32/release/moai-dll.dll sdk/bin/
cp -R ../../vs2008/bin/Win32/release/moai-dll.lib sdk/bin/
cp -R ../../vs2008/bin/Win32/release/moai.exe sdk/bin/
cp -R ../../vs2008/bin/Win32/release/lua-5.1.3.dll sdk/bin/
cp -R ../../vs2008/bin/Win32/release/lua-5.1.3.dll.lib sdk/bin/
cp -R ../../vs2008/bin/Win32/release/glut32.dll sdk/bin/

cp -R ../doxygen/html-lua/html sdk/docs/html/
cp -R ../../samples/basics sdk/samples/lua/
cp -R ../../samples/hello-moai sdk/samples/lua/hello-moai/
cp -R ../../samples/config sdk/samples/lua/config/
cp -R ../../src/aku/AKUGlut.* sdk/samples/hosts/src/aku/

cp -R ../../src/aku/AKU.h sdk/include/aku/