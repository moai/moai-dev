#!/bin/bash
rm -rf libs
rm -rf obj

cd libcrypto-windows
./clean.sh

echo libmoai cleaned.