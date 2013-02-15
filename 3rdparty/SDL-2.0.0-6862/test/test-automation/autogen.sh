#! /bin/sh

echo "Generating build information using autoreconf"
echo "This may take a while..."

test -d acinclude || mkdir acinclude

autoreconf -i

# Run configure for this platform
echo "Now you are ready to run ./configure"
