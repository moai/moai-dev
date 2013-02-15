#! /bin/bash

DIRECTORY="tests"
EXT="error"

PLATFORM="$(uname)"
if [[ $PLATFORM ==  "Linux" ]]; then
	EXT="so"
elif [[ $PLATFORM ==  "Darwin" ]]; then
	EXT="dylib"
fi

echo "Debug: $DIRECTORY/*.$EXT"

rm -f $DIRECTORY/*.$EXT
rm -f runner
rm -f *.bmp
