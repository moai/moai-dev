#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e
cd `dirname $0`

for f in $(find ../samples -type f -name run.bat); do	
	echo "Replacing $f";
	rm -f "${f%/*}/run.bat";
	cp -f "run-scripts/run.bat" "${f%/*}/run.bat";
	rm -f "${f%/*}/run.sh";
	cp -f "run-scripts/run.sh" "${f%/*}/run.sh";
done