#!/bin/bash

source ~/.bash_profile

cd `dirname $0`/..
cd ant/libmoai
./clean.sh
if [[ $? -ne 0 ]]; then
    exit 1
fi

./build.sh $1
if [[ $? -ne 0 ]]; then
    exit 1
fi


## Create default host
cd ..
./make-host.sh -p com.getmoai.samples -s $1
if [[ $? -ne 0 ]]; then
    exit 1
fi

cd untitled-host

cd ../..
rm -rf release/android
mkdir -p release/android/lib/
mkdir -p release/android/host/
cp -rf ant/libmoai/libs/** release/android/lib/
cp -rf ant/untitled-host/** release/android/host/