#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

./clean-sdk.sh

# general files
cp -R ../../3rdparty/glut-3.7.6-bin sdk/3rdparty/glut-3.7.6/

cp -R ../../samples/basics sdk/samples/lua/
cp -R ../../samples/hello-moai sdk/samples/lua/hello-moai/
cp -R ../../samples/config sdk/samples/lua/config/

cp -R ../../src/aku/*.h sdk/include/aku/

cp -R ../../src/samples sdk/samples/hosts/src

# android host
cp -R ../../eclipse/android-project sdk/samples/hosts/eclipse/android-project/
