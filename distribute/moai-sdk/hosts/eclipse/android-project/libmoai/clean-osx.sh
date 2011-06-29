#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#
#!/bin/bash

echo ----------------------------------------------------------------
echo Cleaning libmoai...
echo ----------------------------------------------------------------

rm -rf libs
rm -rf obj

cd libcrypto-osx
./clean.sh

echo libmoai cleaned.