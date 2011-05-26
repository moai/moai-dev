#!/bin/bash
rm -rf libs
rm -rf obj

cd libcrypto-osx
./clean.sh

echo libmoai cleaned.