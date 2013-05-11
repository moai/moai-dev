#! /bin/bash

cp src/runner/.libs/runner .
chmod u+x runner

PLATFORM="$(uname)"
if [[ $PLATFORM ==  "Linux" ]]; then
	cp -f src/libSDLtest/.libs/libSDLtest.so.0 /usr/local/lib
elif [[ $PLATFORM ==  "Darwin" ]]; then
	cp -f src/libSDLtest/.libs/libSDLtest.0.dylib /usr/local/lib
fi

echo "Runner installed."
