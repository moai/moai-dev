#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	rm -f jni/src/packaged-moai.cpp
	rm -rf `find . -type d -name libs`
	rm -rf `find . -type d -name obj`
