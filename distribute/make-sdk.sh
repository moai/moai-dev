#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

./clean-sdk.sh

cp -R ../../3rdparty/glut-3.7.6-bin sdk/3rdparty/glut-3.7.6/

cp -R ../../samples/basics sdk/samples/lua/
cp -R ../../samples/hello-moai sdk/samples/lua/hello-moai/
cp -R ../../samples/config sdk/samples/lua/config/

cp -R ../../src/aku/*.h sdk/include/aku/