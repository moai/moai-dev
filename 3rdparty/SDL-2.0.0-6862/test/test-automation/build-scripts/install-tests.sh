#! /bin/bash

DIRECTORY="tests"
EXT="error"

mkdir $DIRECTORY > /dev/null 2>&1

PLATFORM="$(uname)"
if [[ $PLATFORM ==  "Linux" ]]; then
	EXT="so"
elif [[ $PLATFORM ==  "Darwin" ]]; then
	EXT="dylib"
fi

# TODO: put the test in an array
for suite in "testdummy" "testplatform" "testrect" "testaudio" "testsurface" "testrwops" "testrender" "testvideo" "testsyswm" "testclipboard" "testevents" "testkeyboard"
do
	cp -f "$DIRECTORY/$suite/.libs/lib$suite.$EXT" $DIRECTORY
done

echo "Test suites installed."
echo "Run './runner --show-tests' to see the available tests"
echo "and './runner' to execute them"
echo ""
echo "For more commands: './runner --help'"
