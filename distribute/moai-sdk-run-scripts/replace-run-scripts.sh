#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e

for f in $(find ../moai-sdk/samples -type f -name run.bat); do	
	echo "Replacing $f";
	rm -f ${f%/*}/run.bat;
	cp -f run.bat ${f%/*}/run.bat;
	rm -f ${f%/*}/run.sh;
	cp -f run.sh ${f%/*}/run.sh;
done